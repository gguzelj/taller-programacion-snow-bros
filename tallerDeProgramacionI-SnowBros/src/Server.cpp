#include "../headers/Server.h"

/**
 * Constructor del Server
 */
Server::Server() {

	acceptNewClients_ = true;
	running_ = false;
	sockfd_ = 0;
	port_ = 0;
	connectionsLimit_ = 0;
	model_ = nullptr;
	parser_ = nullptr;
	jsonPath_ = "";
	shared_rcv_queue_ = new Threadsafe_queue<receivedData_t*>();

	//Inicializo el generador de randoms
	srand(static_cast<unsigned>(time(0)));

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
		return SRV_ERROR;

	model_ = new Escenario(parser_);

	connectionsLimit_ = parser_->getConnectionsLimit();

	float xIni = 1;
	float yIni;

	for (int i = 0; i < connectionsLimit_; i++) {
		xIni = i * 7;
		yIni = getInitialY();
		model_->crearPersonaje(xIni, yIni, "sin asignar");
	}

	return SRV_NO_ERROR;
}

/**
 * Metodo para crear el socket con el que va a trabajar el server
 */
int Server::createSocket() {

	Log::instance()->append("Creando Socket", Log::INFO);

	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd_ < 0) {
		Log::instance()->append("Error al tratar de abrir el socket",
				Log::ERROR);
		return SRV_ERROR;
	}

	Log::instance()->append("Socket creado!", Log::INFO);
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
	Log::instance()->append("Socket Bindeado", Log::INFO);
	return SRV_NO_ERROR;
}

/**
 * Thread Principal
 */
void Server::run() {

	listen(sockfd_, connectionsLimit_);

	Log::instance()->append("Corriendo Juego", Log::INFO);

	//Thread para inicializar las conexiones.
	newConnectionsThread_ = std::thread(&Server::newConnectionsManager, this);

	running_ = true;
	while (running_) {

		//La sincronizacion de threads se producira gracias a la shared_rcv_queue_
		//la cual permite ejecutar los eventos recibidos por orden de llegada
		step();
		enviarAClientes();
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

	Log::instance()->append("Corriendo thread para aceptar conexiones",
			Log::INFO);

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
	int aviso;
	unsigned int index;
	connection_t connection;
	Threadsafe_queue<dataToSend_t>* personal_queue;

	Log::instance()->append("Nuevo cliente conectado!", Log::INFO);

	try {

		//Recibimos el id del cliente
		recvall(newsockfd, &connection.id, sizeof(conn_id));
		connection.activa = true;
		connection.socket = newsockfd;

		//Si no encontramos lugar para guardar el nuevo cliente, lo informamos
		//Ademas obtenemos en index donde se guarda la conexion, para utilizar
		//al agregar la personal queue
		if (searchPlaceForConnection(connection, index)) {

			aviso = SRV_NO_ERROR;
			sendall(connection.socket, &aviso, sizeof(int));

		} else {

			aviso = SRV_ERROR;
			sendall(connection.socket, &aviso, sizeof(int));

			return SRV_ERROR;
		}

		//Creamos el personaje en el mundo
		model_->asignarPersonaje(connection.id);
		msg = "Se asigno un personaje a la conexion ";
		msg += connection.id;
		Log::instance()->append(msg, Log::INFO);

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

	} catch (const sendException& e) {
		return SRV_ERROR;

	} catch (const receiveException& e) {
		return SRV_ERROR;
	}

	return SRV_NO_ERROR;
}

/**
 * Buscamos si existe lugar disponible para una nueva conexion:
 * Si la cantidad de conexiones es mayor o igual al limite, se rechaza la nueva conexion.
 * Si el usuario ya estuvo conectado pero se encontraba inacctivo, se reestablece la conexion
 */
bool Server::searchPlaceForConnection(connection_t conn, unsigned int &index) {

	std::string msg;

	msg = "Buscamos lugar para la conexion con ID ";
	msg += conn.id;
	Log::instance()->append(msg, Log::INFO);

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
	if (connections_.size() < connectionsLimit_) {

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

	std::string msg;

	Log::instance()->append("Enviamos la cant de objetos creados", Log::INFO);

	try {

		datos_.cantPersonajes = model_->getCantPersonajes();
		datos_.cantObjDinamicos = model_->getCantObjDinamicos();
		datos_.cantObjEstaticos = model_->getCantObjEstaticos();

		//Enviamos la cantidad de objetos creados en el juego
		sendall(sockfd, &datos_, sizeof(datos_));

		//Enviamos la lista de objetos
		enviarEstaticos(sockfd, model_->getObjetosEstaticos());
		enviarDinamicos(sockfd, model_->getObjetosDinamicos());
		enviarPersonajes(sockfd, model_->getPersonajesParaEnvio());

	} catch (const sendException& e) {
		return;
	}
}

/**
 * Por cada cliente vamos a correr este metodo que se encarga de guardar
 * los mensajes que recibe en la cola compartida
 */
void Server::recibirDelCliente(connection_t conn) {

	std::string msg;
	receivedData_t* data;

	while (running_) {

		data = (receivedData_t*) malloc(sizeof(receivedData_t));

		try {

			recvall(conn.socket, data, sizeof(receivedData_t));

		} catch (const receiveException& e) {

			free(data);
			conn.activa = false;

			msg = "No se pueden recibir datos de la conexion ";
			msg += conn.id;
			Log::instance()->append(msg, Log::WARNING);

			return;
		}

		shared_rcv_queue_->push(data);

	}

}

/**
 * Metodo encargado de enviar los datos a los distintos clientes.
 * Insertamos la informacion en una cola que utilizan los distintos
 * threads
 */
void Server::enviarAClientes() {

	dataToSend_t dataToBeSent;

	dataToBeSent.personajes = model_->getPersonajesParaEnvio();
	dataToBeSent.dinamicos = model_->getObjetosDinamicos();

	for (unsigned int i = 0; i < connections_.size(); i++) {

		if (!connections_[i].activa)
			continue;

		per_thread_snd_queues_[i]->push(dataToBeSent);
	}
}

/**
 * Thread que corre por cada cliente para enviarle informacion
 * del juego
 */
void Server::enviarAlCliente(connection_t conn,
		Threadsafe_queue<dataToSend_t>* personal_queue) {

	std::string msg;
	dataToSend_t dataToBeSent;

	while (true) {

		personal_queue->wait_and_pop(dataToBeSent);

		try {

			enviarPersonajes(conn.socket, dataToBeSent.personajes);
			enviarDinamicos(conn.socket, dataToBeSent.dinamicos);

		} catch (const sendException& e) {

			conn.activa = false;

			msg = "No se pueden enviar datos a la conexion ";
			msg += conn.id;
			Log::instance()->append(msg, Log::WARNING);

			return;
		}

	}

}

/**
 * Metodo encargado de hacer el step del gameloop
 */
void Server::step() {
	receivedData_t* data;

	if (shared_rcv_queue_->try_pop(data)) {

		//Buscamos el personaje y procesamos sus eventos
		Personaje* personaje = model_->getPersonaje(data->id);
		if (!personaje)
			Log::instance()->append(
					"No se puede mapear el personaje con uno del juego",
					Log::WARNING);

		else {

			personaje->handleInput(data->keycode_1, data->type_1);
			personaje->handleInput(data->keycode_2, data->type_2);

		}

	}

	//process(data)
	model_->step();
	SDL_Delay(50);
}

/**
 * Validaciones de parametros.
 * Por ahora solo se toma el nro de puerto. En caso de ser necesarias
 * mas validaciones, modificamos este metodo
 */
int Server::validateParameters(int argc, char *argv[]) {

	Log::instance()->append("Validamos Parametros", Log::INFO);

	//Validamos cantidad de parametros
	if (argc != 3) {
		Log::instance()->append("Cantidad de parametros incorrecta",
				Log::WARNING);
		return SRV_ERROR;
	}

	port_ = atoi(argv[2]);
	if (port_ == 0)
		return SRV_ERROR;

	jsonPath_ = argv[1];

	Log::instance()->append("Parametros correctos!", Log::INFO);

	return SRV_NO_ERROR;
}

/**
 * Asignamos una posicion inicial para un personaje que se esta creando
 */
float Server::getInitialX() {
	float LO = -model_->getAnchoUn() / 2 + 1;
	float HI = model_->getAnchoUn() / 2 - 1;

	//This will generate a number from some arbitrary LO to some arbitrary HI
	return (LO
			+ static_cast<float>(rand())
					/ (static_cast<float>(RAND_MAX / (HI - LO))));
}

/**
 * Asignamos una posicion incial para un personaje que se esta creando
 */
float Server::getInitialY() {
	return 0.0;
}

/**
 * Metodo para enviar los objetos dinamicos
 */
void Server::enviarDinamicos(int sock, figura_t* dinamicos) {

	int size = sizeof(figura_t) * datos_.cantObjDinamicos;
	sendall(sock, dinamicos, size);

}

/**
 * Metodo para enviar los objetos estaticos
 */
void Server::enviarEstaticos(int sock, figura_t* estaticos) {

	int size = sizeof(figura_t) * datos_.cantObjEstaticos;
	sendall(sock, estaticos, size);

}

/**
 * Metodo para enviar los personajes
 */
void Server::enviarPersonajes(int sock, personaje_t* personajes) {

	int size = sizeof(personaje_t) * datos_.cantPersonajes;
	sendall(sock, personajes, size);

}

/**
 * Metodo encargado de hacer el envio de informacion
 */
void Server::sendall(int s, void* data, int len) throw (sendException) {
	int total = 0;
	int bytesleft = len;
	int n;

	while (total < len) {
		n = send(s, data + total, bytesleft, 0);

		//Si aparece un error al enviar, lanzamos una excepcion
		if (n == -1) {
			Log::instance()->append("Error al enviar al cliente", Log::ERROR);
			throw sendException();
		}

		total += n;
		bytesleft -= n;
	}
	return;
}

/**
 * Metodo encargado de recibir la informacion
 */
void Server::recvall(int s, void *data, int len) throw (receiveException) {

	int total = 0;
	int bytesleft = len;
	int n;

	while (total < len) {
		n = recv(s, data + total, bytesleft, 0);

		//Si aparece un error al recibir, lanzamos una excepcion
		if (n == -1 || n == 0) {
			Log::instance()->append("Error al leer del Cliente", Log::ERROR);
			throw receiveException();
		}

		total += n;
		bytesleft -= n;
	}

	return;
}
