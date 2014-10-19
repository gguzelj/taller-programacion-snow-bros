#include "../headers/Server.h"

/**
 * Constructor del Server
 */
Server::Server() {

	acceptNewClients_ = true;
	running_ = false;
	sockfd_ = 0;
	port_ = 0;
	model_ = nullptr;
	parser_ = nullptr;
	jsonPath_ = "";
	shared_rcv_queue_ = new Threadsafe_queue<receivedData_t*>();

	Log::instance()->loggerLevel = Log::INFO;
	Log::instance()->append("Creando SERVER!", Log::INFO);
}

/**
 * Destructor del server
 */
Server::~Server() {
	//Dejamos de aceptar nuevos clientes
	acceptNewClients_ = false;
	newConnectionsThread_.join();

	//Cerramos el socket abierto
	close(sockfd_);
}

/**
 * Inicializamos el servidor
 */
int Server::init(int argc, char *argv[]) {

	//Validamos los parametros con los que se ejecuto el server
	if (validateParameters(argc, argv) == SRV_ERROR)
		return SRV_ERROR;

	//Creamos el socket
	if (createSocket() == SRV_ERROR)
		return SRV_ERROR;

	//Bindeamos el socket
	if (bindSocket() == SRV_ERROR)
		return SRV_ERROR;

	//Creamos el juego
	parser_ = new JsonParser(jsonPath_);
	if (parser_->parse())
		return false;

	model_ = new Escenario(parser_);

	return SRV_NO_ERROR;
}

/**
 * Metodo para crear el socket con el que va a trabajar el server
 */
int Server::createSocket() {

	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd_ < 0) {
		Log::instance()->append("Error al tratar de abrir el socket",
				Log::ERROR);
		return SRV_ERROR;
	}

	return SRV_NO_ERROR;
}

/**
 * Bindeamos el socket
 */
int Server::bindSocket() {
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof serv_addr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_);

	if (bind(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		Log::instance()->append("Error al tratar de bindear el socket",
				Log::ERROR);
		close(sockfd_);
		return SRV_ERROR;
	}
	return SRV_NO_ERROR;
}

/**
 * Thread Principal
 */
void Server::run() {

	listen(sockfd_, BACKLOG);

	//Thread para inicializar las conexiones.
	//Dentro se inicializan los threads para recepcion y envio
	newConnectionsThread_ = std::thread(&Server::newConnectionsManager, this);

	running_ = true;
	while (running_) {

		//La sincronizacion de threads se producira gracias a la shared_rcv_queue_
		//la cual permite ejecutar los eventos recibidos por orden de llegada
		step();
		prepararEnvio();
	}

	Log::instance()->append("Fin del juego", Log::INFO);
	return;
}

/*
 * Metodo que se encarga de inicializar una conexion con el cliente.
 * Si el cliente es aceptado, lanza los threads de enviar y recibir datos, correspondientes al nuevo cliente.
 * A su vez se guarda dicho socket en un vector de sockets.
 * Este metodo ademas seria el encargado de autenticar a los clientes y mandarle la info inicial del juego
 */
void Server::newConnectionsManager() {
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	while (acceptNewClients_) {

		//Aceptamos una nueva conexion
		int newsockfd = accept(sockfd_, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0) {
			Log::instance()->append("Al aceptar nueva conexion", Log::ERROR);
			continue;
		}

		//Validamos si es una conexion correcta
		if (acceptConnection(newsockfd) == SRV_ERROR)
			continue;

	}
}

/**
 * Metodo encargado de negociar la conexion con el cliente
 */
int Server::acceptConnection(int newsockfd) {

	std::string msg;
	int size = sizeof(conn_id);
	unsigned int index;
	connection_t connection;
	Threadsafe_queue<dataToSend_t>* personal_queue;

	Log::instance()->append("Nuevo cliente conectado!", Log::INFO);

	//Recibimos el id del cliente
	if (recvall(newsockfd, &connection.id, &size) <= 0) {

		msg = "No se pudo leer el ID del cliente";
		Log::instance()->append(msg, Log::ERROR);
		return SRV_ERROR;

	}

	connection.activa = true;
	connection.socket = newsockfd;

	//Si no encontramos lugar para guardar el nuevo cliente, lo informamos
	//Ademas obtenemos en index donde se guarda la conexion, para utilizar
	//al agregar la personal queue
	if (!searchPlaceForConnection(connection, index))
		return SRV_ERROR;

	//Comenzamos enviando la informacion del juego
	enviarDatosJuego(newsockfd);

	//Lanza el thread para que el cliente pueda empezar a mandar eventos en forma paralela
	rcv_threads_.push_back(
			std::thread(&Server::recibirDelCliente, this, connection));

	//Crea la queue para el envio de datos del server al cliente y luego lanza el thread
	//para que el server ya pueda mandarle info en forma paralela
	personal_queue = new Threadsafe_queue<dataToSend_t>();

	if (index == per_thread_snd_queues_.size()) {

		per_thread_snd_queues_.push_back(personal_queue);
		snd_threads_.push_back(
				std::thread(&Server::enviarAlCliente, this, connection,
						personal_queue));

	} else {

		per_thread_snd_queues_[index] = personal_queue;
		snd_threads_[index] = std::thread(&Server::enviarAlCliente, this,
				connection, personal_queue);
	}

	return SRV_NO_ERROR;
}

/**
 * Buscamos si existe lugar disponible para una nueva conexion:
 * Si la cantidad de conexiones es mayor o igual al limite,
 * buscamos si existe algun usuario inactivo, y utilizamos ese lugar
 */
bool Server::searchPlaceForConnection(connection_t conn, unsigned int &index) {

	std::string msg;

	//El primer paso consta en buscar alguna conexion con el mismo ID
	for (unsigned int i = 0; i < connections_.size(); i++) {

		//Si encontramos una conexion inactiva con el mismo id, la activamos
		if (connections_[i].id == conn.id) {
			if (connections_[i].activa == false) {
				msg = "Ya existe una conexion inactiva para el ID ";
				msg += conn.id;
				msg += ". Se reactiva";
				Log::instance()->append(msg, Log::INFO);
				connections_[i].activa = true;
				return true;
			} else {
				msg = "Ya existe una conexion activa para el ID ";
				msg += conn.id;
				Log::instance()->append(msg, Log::WARNING);
				return false;
			}

		}

	}

	//Validamos si existe lugar suficiente
	if (connections_.size() < BACKLOG) {

		index = connections_.size();
		connections_.push_back(conn);

		msg = "Agregamos la nueva conexion ";
		msg += conn.id;
		Log::instance()->append(msg, Log::INFO);

		return true;

	}

	msg = "No existe lugar disponible para la nueva conexion ";
	msg += conn.id;
	Log::instance()->append(msg, Log::WARNING);

	return false;
}

/**
 * Metodo utilizado para enviar por primera vez todos los datos del juego
 * a un nuevo cliente
 */
void Server::enviarDatosJuego(int sockfd) {

	int size;
	firstConnectionDetails_t datos;

	//El primer paso es enviar la cantidad de objetos creados en el juego
	datos.cantObjDinamicos = model_->getCantObjDinamicos();
	datos.cantObjEstaticos = model_->getCantObjEstaticos();

	size = sizeof(datos);
	if (sendall(sockfd, &datos, &size) <= 0) {
		Log::instance()->append("No se pueden enviar datos", Log::WARNING);
	}

	//Enviamos la lista de objetos Estaticos
	size = sizeof(objEstatico_t) * model_->getCantObjEstaticos();
	objEstatico_t *objetosEstaticos = model_->getObjetosEstaticos();
	if (sendall(sockfd, objetosEstaticos, &size) <= 0) {
		Log::instance()->append("No se pueden enviar datos", Log::WARNING);
	}

	//Enviamos la lista de los objetos Dinamicos
	size = sizeof(objDinamico_t) * model_->getCantObjDinamicos();
	objDinamico_t *objetosDinamicos = model_->getObjetosDinamicos();
	if (sendall(sockfd, objetosDinamicos, &size) <= 0) {
		Log::instance()->append("No se pueden enviar datos", Log::WARNING);
	}

}

/**
 * Por cada cliente vamos a correr este metodo que se encarga de guardar
 * los mensajes que recibe en la cola compartida
 */
void Server::recibirDelCliente(connection_t conn) {

	int size = sizeof(receivedData_t);
	std::string msg;

	while (true) {

		receivedData_t* data = (receivedData_t*) malloc(size);

		if (recvall(conn.socket, data, &size) <= 0) {

			//Si no el cliente se desconecta, lo desactivamos
			msg = "No se pueden recibir datos de la conexion ";
			msg += conn.id;
			Log::instance()->append(msg, Log::WARNING);

			conn.activa = false;
			return;

		}

		// Al hacer push se notifica al step mediante una condition_variable
		//que tiene data para procesar
		shared_rcv_queue_->push(data);
	}
}

void Server::prepararEnvio() {
	dataToSend_t dataToBeSent;
//
// Crea el struct y lo adapta a lo que hay que mandar
//
// Inserta el dato a mandar en la cola de envios de cada thread
	for (unsigned int i = 0; i < connections_.size(); i++) {

		//Ignoramos las conexiones que no estan activas
		if (!connections_[i].activa)
			continue;

		per_thread_snd_queues_[i]->push(dataToBeSent);
		// Al hacer push se notifica al thread mediante una condition_variable que tiene data para enviar
	}
}

void Server::enviarAlCliente(connection_t conn,
		Threadsafe_queue<dataToSend_t>* personal_queue) {

	//wait_and_pop va a esperar a que haya un elemento en la
	//personal_queue para desencolar el mismo.
	while (true) {
		dataToSend_t dataToBeSent;
		personal_queue->wait_and_pop(dataToBeSent);
		int size = sizeof(dataToSend_t);

		if (sendall(conn.socket, &dataToBeSent, &size) < 0) {
			std::cout << "No pude enviar. Client disconnected from server";
			throw;
		}
	}
}

/**
 * Metodo encargado de hacer el step del gameloop
 */
void Server::step() {
	receivedData_t* event;
	shared_rcv_queue_->wait_and_pop(event);
//process(data)

	model_->step();
}

/**
 * Validaciones de parametros.
 * Por ahora solo se toma el nro de puerto. En caso de ser necesarias
 * mas validaciones, modificamos este metodo
 */
int Server::validateParameters(int argc, char *argv[]) {

	Log::instance()->append("Validamos Parametros", Log::INFO);

//Validamos cantidad de parametros. En caso de que no sean correctos, se
//comienza con un juego default
	if (argc != 3) {
		Log::instance()->append("Cantidad de parametros incorrecta",
				Log::WARNING);
		return SRV_ERROR;
	}

	port_ = atoi(argv[2]);
	if (port_ == 0)
		return SRV_ERROR;

	jsonPath_ = argv[1];

	return SRV_NO_ERROR;
}

/**
 * Metodo encargado de hacer el envio de informacion
 */
int Server::sendall(int s, void* data, int* len) {
	int total = 0; 			// how many bytes we've sent
	int bytesleft = *len; 	// how many we have left to send
	int n;
	while (total < *len) {
		n = send(s, data + total, bytesleft, 0);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
	*len = total;      		// return number actually sent here
	return n == -1 ? -1 : 0; 		// return -1 on failure, 0 on success
}

/**
 * Metodo encargado de recibir la informacion
 */
int Server::recvall(int s, void *data, int *len) {

	int total = 0; 			// how many bytes we've recieve
	int bytesleft = *len; 	// how many we have left to recieve
	int n;

	while (total < *len) {
		n = recv(s, data + total, bytesleft, 0);
		if (n == -1 || n == 0) {
			break;
		}
		total += n;
		bytesleft -= n;
	}

	return n == -1 || n == 0 ? -1 : 0; 	// return -1 on failure, 0 on success
}
