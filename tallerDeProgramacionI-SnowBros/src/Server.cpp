#include "../headers/Server.h"

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

	gameData_.paused = false;

	//Inicializo el generador de randoms
	srand(static_cast<unsigned>(time(0)));

	Log::ins()->loggerLevel = Log::INFO;
	Log::ins()->add(SRV_MSG_CREATE, Log::INFO);
}

Server::~Server() {

	acceptNewClients_ = false;
	newConnectionsThread_.join();

	close(sockfd_);
}

int Server::init(int argc, char *argv[]) {

	if (validateParameters(argc, argv) == SRV_ERROR)
		return SRV_ERROR;

	if (createSocket() == SRV_ERROR)
		return SRV_ERROR;

	if (bindSocket() == SRV_ERROR)
		return SRV_ERROR;

	parser_ = new JsonParser(jsonPath_);
	if (parser_->parse())
		return SRV_ERROR;

	model_ = new Escenario(parser_);

	connectionsLimit_ = parser_->getConnectionsLimit();

	return SRV_NO_ERROR;
}

int Server::createSocket() {

	struct timeval timeout;
	timeout.tv_sec = HB_TIMEOUT;
	timeout.tv_usec = 0;

	Log::ins()->add(SRV_MSG_SOCK_CREATE, Log::INFO);

	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd_ < 0) {
		Log::ins()->add(SRV_MSG_SOCK_ERROR, Log::ERROR);
		return SRV_ERROR;
	}

	//Si el socket se creo correctamente, agregamos el timeout
	if (setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
		return SRV_ERROR;
	}

	if (setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
		return SRV_ERROR;
	}

	Log::ins()->add(SRV_MSG_SOCK_OK, Log::INFO);
	return SRV_NO_ERROR;
}

int Server::bindSocket() {
	struct sockaddr_in serv_addr;

	Log::ins()->add(SRV_MSG_BIND_CREATE, Log::INFO);

	memset(&serv_addr, 0, sizeof serv_addr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port_);

	if (bind(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {

		Log::ins()->add(SRV_MSG_BIND_ERROR, Log::ERROR);
		close(sockfd_);
		return SRV_ERROR;

	}

	Log::ins()->add(SRV_MSG_BIND_OK, Log::INFO);
	return SRV_NO_ERROR;
}

/*
 * Thread Principal
 */
void Server::run() {

	listen(sockfd_, connectionsLimit_);

	Log::ins()->add(SRV_MSG_RUNNING_GAME, Log::INFO);

	//Thread para inicializar las conexiones.
	newConnectionsThread_ = std::thread(&Server::newConnectionsManager, this);

	running_ = true;
	while (running_) {

		step();
		enviarAClientes();

	}

	Log::ins()->add(SRV_MSG_END_GAME, Log::INFO);
	return;
}

void Server::newConnectionsManager() {
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	Log::ins()->add(SRV_MSG_CONN_MAN, Log::INFO);

	while (acceptNewClients_) {
		//Aceptamos una nueva conexion
		int newsockfd = accept(sockfd_, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0)
			continue;

		//Validamos si es posible aceptar la nueva conexion
		if (acceptConnection(newsockfd) == SRV_ERROR)
			continue;

		//Si la conexion se acepto, validamos si puede comenzar el juego
		if (validarComienzoJuego() == SRV_ERROR)
			continue;
	}
}

int Server::acceptConnection(int newsockfd) {

	std::string msg;
	bool reconexion;
	connection_t* connection = (connection_t*) malloc(sizeof(connection_t));

	Log::ins()->add(SRV_MSG_NEW_CLIENT, Log::INFO);

	try {

		//Recibimos el id del cliente
		recvall(newsockfd, &(connection->id), sizeof(conn_id));
		connection->activa = true;
		connection->socket = newsockfd;

		//Buscamos un lugar para la nueva conexion
		if (esNuevoCliente(connection->id)) {

			reconexion = false;

			if (manejarNuevoCliente(connection) == SRV_ERROR)
				return SRV_ERROR;

		} else {

			reconexion = true;

			if (manejarReconexion(connection) == SRV_ERROR)
				return SRV_ERROR;

		}

		//Creamos el personaje y enviamos los datos del juego
		crearPersonaje(connection, reconexion);
		enviarDatosJuego(newsockfd);
		initReceivingThread(connection);
		initSendingThread(connection);

	} catch (const sendException& e) {
		return SRV_ERROR;

	} catch (const receiveException& e) {
		return SRV_ERROR;
	}

	return SRV_NO_ERROR;
}

int Server::manejarNuevoCliente(connection_t *conn) {

	std::string msg;
	int aviso;

	Log::ins()->add(SRV_MSG_SEARCH_POS + std::string(conn->id), Log::INFO);

	//Validamos si existe lugar suficiente
	if (connections_.size() < connectionsLimit_) {

		connections_.push_back(conn);

		aviso = SRV_NO_ERROR;
		sendall(conn->socket, &aviso, sizeof(int));

		Log::ins()->add(SRV_MSG_ADD_CONN + std::string(conn->id), Log::INFO);

		return SRV_NO_ERROR;

	}

	aviso = SRV_ERROR;
	sendall(conn->socket, &aviso, sizeof(int));

	Log::ins()->add(SRV_MSG_REJ_CONN + std::string(conn->id), Log::WARNING);

	return SRV_ERROR;
}

int Server::manejarReconexion(connection_t *conn) {

	std::string msg;
	int aviso;

	Log::ins()->add(SRV_MSG_TRY_RECONN + std::string(conn->id), Log::INFO);

	//El primer paso consta en buscar alguna conexion con el mismo ID
	for (unsigned int i = 0; i < connections_.size(); i++) {

		//Si encontramos una conexion inactiva con el mismo id, la activamos
		if (strcmp(connections_[i]->id, conn->id) == 0) {

			if (connections_[i]->activa == false) {

				aviso = SRV_NO_ERROR;
				sendall(conn->socket, &aviso, sizeof(int));

				Log::ins()->add(SRV_MSG_RECONN + std::string(conn->id), Log::INFO);

				connections_[i] = conn;

				return SRV_NO_ERROR;

			} else {

				aviso = SRV_ERROR;
				sendall(conn->socket, &aviso, sizeof(int));

				Log::ins()->add(SRV_MSG_RECONN_ERROR + std::string(conn->id), Log::WARNING);

				return SRV_ERROR;
			}
		}
	}

	aviso = SRV_ERROR;
	sendall(conn->socket, &aviso, sizeof(int));

	return SRV_ERROR;

}

/*
 * Si se llama este metodo es porque se acepto un nuevo cliente.
 * Eso significa que no se llego al limite maximo de jugadores, y que
 * por lo tanto, el juego estaba pausado.
 * Validamos el limite de jugadores, y empezamos a correr el juego si
 * corresponde
 */
int Server::validarComienzoJuego() {

	if (connections_.size() == connectionsLimit_) {
		gameData_.paused = false;
		return SRV_NO_ERROR;
	}

	return SRV_ERROR;
}

/*
 * Metodo utilizado para enviar por primera vez todos los datos del juego
 * a un nuevo cliente
 */
void Server::enviarDatosJuego(int sockfd) {

	try {

		datos_.cantPersonajes = model_->getCantPersonajes();
		datos_.cantEnemigos = model_->getCantEnemigos();
		datos_.cantObjDinamicos = model_->getCantObjDinamicos();
		datos_.cantObjEstaticos = model_->getCantObjEstaticos();

		//Enviamos la cantidad de objetos creados en el juego
		sendall(sockfd, &datos_, sizeof(datos_));

		//Enviamos la lista de objetos
		enviarEstaticos(sockfd, model_->getObjetosEstaticos());
		enviarDinamicos(sockfd, model_->getObjetosDinamicos());
		enviarPersonajes(sockfd, model_->getPersonajesParaEnvio());
		enviarEnemigos(sockfd, model_->getEnemigosParaEnvio());

		Log::ins()->add(SRV_MSG_INIT_DATA, Log::INFO);

	} catch (const sendException& e) {
		return;
	}
}

/*
 * Por cada cliente vamos a correr este metodo que se encarga de recibir los
 * mensajes. Los mensajes que se envian pueden ser de dos tipos:
 *
 * -HeartBeat: 	Mensaje para avisar al cliente que sigue conectado
 * -Eventos:	Eventos para que se actualice el personaje en el juego
 *
 * Si no recibimos datos de algun cliente, pausamos el juego
 */
void Server::recibirDelCliente(connection_t *conn) {

	char msgType;
	receivedData_t* data;
	bool recibirDelCliente = true;

	while (recibirDelCliente) {

		data = nullptr;

		try {

			//Primero recibimos el tipo de mensaje
			recvall(conn->socket, &msgType, sizeof(msgType));

			if (msgType == EVENT_MSG_TYPE) {

				data = (receivedData_t*) malloc(sizeof(receivedData_t));
				recvall(conn->socket, data, sizeof(receivedData_t));
				shared_rcv_queue_->push(data);

			} else if (msgType == HB_MSG_TYPE) {
				continue;
			}

		} catch (const receiveException& e) {

			if (data)
				free(data);

			gameData_.paused = true;

			conn->activa = false;
			model_->setPersonajeConnectionState(conn->id, ESPERANDO);

			Log::ins()->add(
			SRV_MSG_RECEIVE_ERROR + std::string(conn->id) + ". Se desactiva la conexion", Log::WARNING);

			recibirDelCliente = false;

		}
	}
}

/*
 * Metodo encargado de enviar los datos a los distintos clientes.
 * Insertamos la informacion en una cola que utilizan los distintos
 * threads
 */
void Server::enviarAClientes() {

	dataToSend_t dataToBeSent;

	gameData_.cantProyectiles = model_->getCantProyectiles();
	gameData_.cantEnemigos = model_->getCantEnemigos();
	gameData_.cantDinamicos = model_->getCantObjDinamicos();
	gameData_.cantSonidos = model_->getCantSonidos();

	dataToBeSent.gameData = &gameData_;
	dataToBeSent.personajes = model_->getPersonajesParaEnvio();
	dataToBeSent.enemigos = model_->getEnemigosParaEnvio();
	dataToBeSent.dinamicos = model_->getObjetosDinamicos();
	dataToBeSent.proyectiles = model_->getProyectiles();
	dataToBeSent.sonidos = model_->getSonidosParaEnvio();
	for (unsigned int i = 0; i < connections_.size(); i++) {

		if (!connections_[i]->activa)
			continue;

		connections_[i]->dataQueue->push(dataToBeSent);
	}
}

/*
 * Thread que corre por cada cliente para enviarle informacion
 * del juego
 */
void Server::enviarAlCliente(connection_t *conn) {

	std::string msg;
	dataToSend_t dataToBeSent;
	bool enviarAlCliente = true;

	while (enviarAlCliente) {

		conn->dataQueue->wait_and_pop(dataToBeSent);

		try {

			enviarGameData(conn->socket, dataToBeSent.gameData);
			enviarPersonajes(conn->socket, dataToBeSent.personajes);
			enviarEnemigos(conn->socket, dataToBeSent.enemigos);
			enviarDinamicos(conn->socket, dataToBeSent.dinamicos);
			enviarProyectiles(conn->socket, dataToBeSent.proyectiles);
			enviarSonidos(conn->socket, dataToBeSent.sonidos);

		} catch (const sendException& e) {

			conn->activa = false;
			model_->setPersonajeConnectionState(conn->id, ESPERANDO);

			Log::ins()->add(SRV_MSG_SEND_ERROR + std::string(conn->id), Log::WARNING);

			enviarAlCliente = false;

		}
	}
}

/*
 * Metodo encargado de hacer el step del gameloop
 */
void Server::step() {
	receivedData_t* data;

	if (shared_rcv_queue_->try_pop(data)) {

		//Descartamos los eventos mientras el juego esta pausado
		if (!gameData_.paused) {

			//Buscamos el personaje y procesamos sus eventos
			Personaje* personaje = model_->getPersonaje(data->id);

			if (!personaje) {
				Log::ins()->add(SRV_MSG_PER_MAP + std::string(data->id), Log::WARNING);
			} else {
				if (data != nullptr) {
					personaje->handleInput(data->keycode_1, data->type_1);
					personaje->handleInput(data->keycode_2, data->type_2);
					personaje->handleInput(data->keycode_3, data->type_3);
				}
			}

		}

	}

	//Solo simulamos mientras el juego no esta pausado
	if (!gameData_.paused)
		model_->step();

	SDL_Delay(30);
}

/*
 * Validaciones de parametros.
 * Por ahora solo se toma el nro de puerto. En caso de ser necesarias
 * mas validaciones, modificamos este metodo
 */
int Server::validateParameters(int argc, char *argv[]) {

	Log::ins()->add(SRV_MSG_VAL_PARAM, Log::INFO);

	//Validamos cantidad de parametros
	if (argc != 3) {
		Log::ins()->add(SRV_MSG_PARAM_INC, Log::WARNING);
		return SRV_ERROR;
	}

	port_ = atoi(argv[2]);
	if (port_ == 0)
		return SRV_ERROR;

	jsonPath_ = argv[1];

	Log::ins()->add(SRV_MSG_PARAM_OK, Log::INFO);

	return SRV_NO_ERROR;
}

float Server::getInitialX() {
	float LO = -(model_->getAnchoUn() / 2) + 10;
	float HI = (model_->getAnchoUn() / 2) - 10;

	//This will generate a number from some arbitrary LO to some arbitrary HI
	return (LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO))));
}

float Server::getInitialY() {
	return -10.0;
}

void Server::enviarProyectiles(int sock, proyectil_t* dinamicos) {
	int size = sizeof(proyectil_t) * gameData_.cantProyectiles;
	sendall(sock, dinamicos, size);
}

void Server::enviarDinamicos(int sock, figura_t* dinamicos) {
	int size = sizeof(figura_t) * gameData_.cantDinamicos;
	sendall(sock, dinamicos, size);
}

void Server::enviarEstaticos(int sock, figura_t* estaticos) {
	int size = sizeof(figura_t) * datos_.cantObjEstaticos;
	sendall(sock, estaticos, size);
}

void Server::enviarGameData(int sock, gameData_t* gameData) {
	sendall(sock, gameData, sizeof(gameData_t));
}

void Server::enviarPersonajes(int sock, personaje_t* personajes) {
	int size = sizeof(personaje_t) * datos_.cantPersonajes;
	sendall(sock, personajes, size);
}

void Server::enviarEnemigos(int sock, enemigo_t* enemigos) {
	int size = sizeof(enemigo_t) * gameData_.cantEnemigos;
	sendall(sock, enemigos, size);
}

void Server::enviarSonidos(int sock, int* sonidos) {
	int size = sizeof(int) * gameData_.cantSonidos;
	sendall(sock, sonidos, size);
}

void Server::sendall(int s, void* data, int len) throw (sendException) {
	int total = 0;
	int bytesleft = len;
	int n;

	while (total < len) {
		n = send(s, data + total, bytesleft, 0);

		//Si aparece un error al enviar, lanzamos una excepcion
		if (n == -1 || n == 0) {
			Log::ins()->add("Ha ocurrido un error en la conexion con el cliente", Log::ERROR);
			throw sendException();
		}

		total += n;
		bytesleft -= n;
	}
	return;
}

void Server::recvall(int s, void *data, int len) throw (receiveException) {

	int total = 0;
	int bytesleft = len;
	int n;

	while (total < len) {
		n = recv(s, data + total, bytesleft, 0);

		//Si aparece un error al recibir, lanzamos una excepcion
		if (n == -1 || n == 0) {
			Log::ins()->add("Ha ocurrido un error en la conexion con el cliente", Log::ERROR);
			throw receiveException();
		}

		total += n;
		bytesleft -= n;
	}

	return;
}

/*
 * Rutina para crear un personaje:
 * -Si es un nuevo cliente, se crea un nuevo personaje
 * -Si es una reconexion, se setea el personaje como conectado
 */
void Server::crearPersonaje(connection_t* connection, bool reconexion) {

	if (reconexion) {
		model_->setPersonajeConnectionState(connection->id, CONECTADO);
	} else {
		model_->crearPersonaje(getInitialX(), getInitialY(), connection->id);
	}

	Log::ins()->add(SRV_MSG_NEW_PER + std::string(connection->id), Log::INFO);
}

/**
 * Validamos si el id es una conexion que ya existe
 */
bool Server::esNuevoCliente(conn_id id) {

	for (unsigned int i = 0; i < connections_.size(); i++)
		if (strcmp(connections_[i]->id, id) == 0)
			return false;

	return true;
}

/**
 * Lanzamos el thread para recibir informacion del cliente
 */
void Server::initReceivingThread(connection_t* connection) {
	std::thread t(&Server::recibirDelCliente, this, connection);
	t.detach();

	Log::ins()->add(SRV_MSG_RECEIVING_THREAD + std::string(connection->id), Log::INFO);
}

/**
 * Lanzamos el thread para enviar informacion al cliente
 */
void Server::initSendingThread(connection_t* connection) {

	connection->dataQueue = new Threadsafe_queue<dataToSend_t>();

	std::thread t(&Server::enviarAlCliente, this, connection);
	t.detach();

	Log::ins()->add(SRV_MSG_SEND_THREAD + std::string(connection->id), Log::INFO);
}
