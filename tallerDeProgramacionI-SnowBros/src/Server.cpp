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

	//With 0 as the protocol, the system chooses automagically the protocol
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
	//Dentro se inicializan los threads para recepcion y envio. Mas info en el header
	newConnectionsThread_ = std::thread(&Server::newConnectionsManager, this);

	running_ = true;
	while (running_) {
		//La sincronizacion de threads se producira gracias a la shared_rcv_queue_ la cual permite ejecutar los eventos recibidos por orden de llegada
		step();
		prepararEnvio();
	}

	std::cout << "Ha finalizado la ejecucion del programa." << std::endl;
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
		int newsockfd = accept(sockfd_, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			Log::instance()->append("Error al aceptar una nueva conexion",
					Log::ERROR);
			continue;
		}
		//Llamar a metodo sincronico para Autenticar el cliente aca TODO

		Log::instance()->append("Nuevo cliente conectado!", Log::INFO);

		sockets_.push_back(newsockfd);

		//Lanza el thread para que el cliente pueda empezar a mandar eventos en forma paralela
		rcv_threads_.push_back(
				std::thread(&Server::recibirDelCliente, this, newsockfd,
						shared_rcv_queue_));

		//Crea la queue para el envio de datos del server al cliente y luego lanza el thread
		//para que el server ya pueda mandarle info en forma paralela
		Threadsafe_queue<dataToSend_t>* personal_queue = new Threadsafe_queue<
				dataToSend_t>();
		per_thread_snd_queues_.push_back(personal_queue);
		snd_threads_.push_back(
				std::thread(&Server::enviarAlCliente, this, newsockfd,
						personal_queue));

		//Aca habria que enviarle al nuevo cliente la lista de todas las cosas del juego (obj estaticos, dinamicos, personajes) TODO
	}
}

void Server::recibirDelCliente(int sock,
		Threadsafe_queue<receivedData_t*>* shared_rcv_queue) {
	while (true) {
		receivedData_t* data = (receivedData_t*) malloc(sizeof(receivedData_t));
		;
		int size = sizeof(receivedData_t);

		if (recvall(sock, data, &size) <= 0) {
			std::cout << "No pude recibir. Client disconnected from server";
			throw;
		}
		std::cout << "Recibi un evento del cliente: " << data->username
				<< std::endl;

		// Al hacer push se notifica al step mediante una condition_variable que tiene data para procesar
		shared_rcv_queue->push(data);
	}
}

int Server::recvall(int s, receivedData_t *data, int *len) {

	int total = 0; 			// how many bytes we've sent
	int bytesleft = *len; 	// how many we have left to send
	int n;

	receivedData_t* original = (receivedData_t*) malloc(sizeof(receivedData_t));
	memcpy(original, data, sizeof(receivedData_t));

	while (total < *len) {
		n = recv(s, original + total, bytesleft, 0);
		if (n == -1 || n == 0) {
			break;
		}
		total += n;
		bytesleft -= n;
	}

	memcpy(data, original, sizeof(receivedData_t));
	free(original);
	*len = total;      				// return number actually sent here
	return n == -1 || n == 0 ? -1 : 0; 	// return -1 on failure, 0 on success
}

void Server::prepararEnvio() {
	dataToSend_t dataToBeSent;
	//
	// Crea el struct y lo adapta a lo que hay que mandar
	//
	// Inserta el dato a mandar en la cola de envios de cada thread
	for (unsigned int i = 0; i < sockets_.size(); i++) {
		Threadsafe_queue<dataToSend_t>* personal_queue =
				per_thread_snd_queues_[i];
		personal_queue->push(dataToBeSent); // Al hacer push se notifica al thread mediante una condition_variable que tiene data para enviar
	}
}

void Server::enviarAlCliente(int sock,
		Threadsafe_queue<dataToSend_t>* personal_queue) {
	while (true) {
		dataToSend_t dataToBeSent;
		personal_queue->wait_and_pop(dataToBeSent);	//wait_and_pop va a esperar a que haya un elemento en la personal_queue para desencolar el mismo. De esta manera hay un sincronizmo y se ahorran recursos si no se usa dicha cola
		int size = sizeof(dataToSend_t);

		if (sendall(sock, &dataToBeSent, &size) < 0) {
			std::cout << "No pude enviar. Client disconnected from server";
			throw;
		}
	}
}

int Server::sendall(int s, dataToSend_t *data, int *len) {
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

// Ver pag 73 - libro c++11
void Server::step() {
	receivedData_t* event;
	shared_rcv_queue_->wait_and_pop(event);
	//process(data)

	model_->getPersonaje()->decreaseJumpCooldown();
	//chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
	//esta implementado aca para que cambie cuando tiene que hacerlo
	if (model_->getPersonaje()->getVelocity().y <= 0.0f
			&& model_->getPersonaje()->getCantidadDeContactosActuales() == 0) {
		model_->getPersonaje()->state = &Personaje::falling;
	} else if (model_->getPersonaje()->getVelocity().y <= 0.0f
			&& model_->getPersonaje()->state == &Personaje::jumping) {
		model_->getPersonaje()->state = &Personaje::standby;
	}

	if (Personaje::walking.movimientoLateralDerecha
			|| Personaje::walking.movimientoLateralIzquierda)
		Personaje::walking.caminar(*(model_->getPersonaje()));

	if (Personaje::jumping.debeSaltar
			&& model_->getPersonaje()->state->getCode() != JUMPING
			&& model_->getPersonaje()->state->getCode() != FALLING) {
		model_->getPersonaje()->jump();
		model_->getPersonaje()->state = &Personaje::jumping;
	}
	model_->getWorld()->Step(timeStep, velocityIterations, positionIterations);
}

/**
 * Validaciones de parametros.
 * Por ahora solo se toma el nro de puerto. En caso de ser necesarias
 * mas validaciones, modificamos este metodo
 */
int Server::validateParameters(int argc, char *argv[]) {

	Log::instance()->append("Validamos Parametros", Log::INFO);

	//Leemos el puerto
	if (argc >= 2)
		port_ = atoi(argv[1]);

	//Validamos cantidad de parametros. En caso de que no sean correctos, se
	//comienza con un juego default
	if (argc != 2) {
		Log::instance()->append("Cantidad de parametros incorrecta",
				Log::WARNING);
		return SRV_ERROR;
	}

	return SRV_NO_ERROR;
}
