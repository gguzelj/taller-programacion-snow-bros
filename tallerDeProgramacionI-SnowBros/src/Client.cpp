#include "../headers/Client.h"

void help();

// ########################## //
// ##### Public methods ##### //
// ########################## //

Client::Client() {
	this->running_ = true;
	//TODO agregar el struct que va a ser el escenario que reciba del server.
	//model_ = nullptr;

	controller_ = nullptr;
	view_ = nullptr;
	host = nullptr;
	name = nullptr;
	estaticos_ = nullptr;
	dinamicos_ = nullptr;
	personajes_ = nullptr;

	port = 0;
	sock = 0;
	shared_rcv_queue_ = new Threadsafe_queue<dataFromServer_t>();

	Log::instance()->loggerLevel = Log::INFO;
	Log::instance()->append(CLIENT_MSG_NEW_CLIENT, Log::INFO);
}

Client::~Client() {
	Log::instance()->append(CLIENT_MSG_DELETE_CLIENT, Log::INFO);
	Log::instance()->closeLog();
}

/*
 * Inicializamos el cliente
 */
bool Client::init(int argc, char* argv[]) {

	if (validateParameters(argc, argv) == CLIENT_ERROR)
		return CLIENT_ERROR;

	if (createSocket() == CLIENT_ERROR)
		return CLIENT_ERROR;

	try {

		Log::instance()->append("Creando vista y controlador", Log::INFO);
		view_ = new Drawer();
		controller_ = new Controlador(view_);
		//TODO cambiar el modelo por lo que reciba del server
		//view_->inicializarCamara(model_->getPersonaje());
		Log::instance()->append("Vista y controlador creados", Log::INFO);

		return CLIENT_OK;

	} catch (exception& e) {
		std::cout << e.what() << std::endl;
		Log::instance()->append(CLIENT_MSG_INIT_ERROR, Log::INFO);
		return CLIENT_ERROR;
	}
}

int Client::run() {

	if (connectToServer() == CLIENT_ERROR)
		return CLIENT_ERROR;

	if (initialize() == CLIENT_ERROR)
		return CLIENT_ERROR;

	//hbTh = std::thread(&Client::enviarHeartBeat, this);
	sendTh = std::thread(&Client::enviarAlServer, this);
	recvTh = std::thread(&Client::recibirDelServer, this);

	//Main thread
	while (running_) {

		//Update the view
		onRender(shared_rcv_queue_->wait_and_pop());

		SDL_Delay(1);
	}

	//cleaning up
	onCleanup();

	return CLIENT_OK;
}

// ########################## //
// ##### Private methods #### //
// ########################## //

/**
 * Creamos el socket utilizado para conectarnos con el servidor
 * Tambien seteamos el HeartBeat Timeout. Esto hara que las funciones
 * send y recv devuelvan un error si no se pudo enviar/recibir un mensaje
 * en el tiempo indicado
 */
int Client::createSocket() {

	struct timeval timeout;
	timeout.tv_sec = HB_TIMEOUT;
	timeout.tv_usec = 0;

	Log::instance()->append("Creando Socket", Log::INFO);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		Log::instance()->append(CLIENT_MSG_SOCK, Log::ERROR);
		return CLIENT_ERROR;
	}
	/*
	 //Si el socket se creo correctamente, agregamos el timeout
	 if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
	 sizeof(timeout)) < 0) {
	 Log::instance()->append(CLIENT_MSG_SOCK_TIMEOUT, Log::ERROR);
	 return CLIENT_ERROR;
	 }

	 if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
	 sizeof(timeout)) < 0) {
	 Log::instance()->append(CLIENT_MSG_SOCK_TIMEOUT, Log::ERROR);
	 return CLIENT_ERROR;
	 }
	 */
	Log::instance()->append("Socket creado!", Log::INFO);
	return CLIENT_OK;
}

/**
 * Metodo utilizado para establecer conexion con el servidor
 */
int Client::connectToServer() {

	std::string msg;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	Log::instance()->append("Intentamos establecer conexion", Log::INFO);

	server = gethostbyname(host);
	if (server == NULL) {
		msg = "No existe el host ";
		msg += host;
		Log::instance()->append(msg, Log::ERROR);
		return CLIENT_ERROR;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		Log::instance()->append(
				"No se pudo establecer conexion con el servidor", Log::WARNING);
		return CLIENT_ERROR;
	}

	msg = "Conexion establecida con el host ";
	msg += host;
	Log::instance()->append(msg, Log::INFO);
	return CLIENT_OK;
}

int Client::initialize() {
	int entro;
	std::string msg;

	try {

		//Envio el nombre de cliente
		sendall(sock, name, 20);

		//Recibo la respuesta del servidor
		recvall(sock, &entro, sizeof(int));

		if (entro == SRV_ERROR) {
			Log::instance()->append("Servidor rechazo la conexion", Log::ERROR);
			return CLIENT_ERROR;
		} else {
			Log::instance()->append("Servidor acepto la conexion", Log::INFO);
		}

		//Recibimos la cantidad de objetos creados en el juego
		recvall(sock, &gameDetails_, sizeof(firstConnectionDetails_t));

		//Recibimos la cantidad de objetos creados
		recibirEstaticos(estaticos_);
		recibirDinamicos(dinamicos_);
		recibirPersonajes(personajes_);

		//Inicializamos la vista para el personaje principal
		for (unsigned int i = 0; i < gameDetails_.cantPersonajes; i++) {

			if (strcmp((personajes_[i]).id, name) == 0) {
				view_->inicializarCamara(personajes_[i]);
			}
		}

	} catch (const sendException& e) {
		return CLIENT_ERROR;

	} catch (const receiveException& e) {
		return CLIENT_ERROR;
	}

	std::cout << "Terminamos de recibir datos de ini" << std::endl;
	return CLIENT_OK;
}

/**
 * Metodo utilizado para enviar señales de vida al servidor.
 */
void Client::enviarHeartBeat() {

	int msgType = HB_MSG_TYPE;
	int size = sizeof(msgType);

}

/**
 * Metodo utilizado para enviar nuevos eventos al servidor
 */
void Client::enviarAlServer() {

	try {

		while (running_) {

			dataToSend_t* data = onEvent();
			if (data->keycode_1 == 0 && data->keycode_2 == 0) {
				free(data);
				continue;
			}

			sendall(sock, data, sizeof(dataToSend_t));

			SDL_Delay(1);
			free(data);
		}

	} catch (const sendException& e) {
		running_ = false;
		Log::instance()->append(CLIENT_MSG_ERROR_WHEN_SENDING, Log::ERROR);
		return;
	}

}

/**
 * Metodo para recibir mensajes del servidor con la informacion de
 * los personajes y los objetos dinamicos. Los objetos estaticos nunca
 * cambian, por lo que no son necesarios
 */
void Client::recibirDelServer() {

	dataFromServer_t data;

	try {

		//La cant de bytes a recibir esta definida por la cantidad de
		//personajes y la cantidad de objetos dinamicos:
		while (running_) {

			//Recibimos los personajes
			recibirPersonajes(data.personajes);

			//Recibimos los objetos dinamicos
			recibirDinamicos(data.dinamicos);

			shared_rcv_queue_->push(data);

		}

	} catch (const sendException& e) {
		running_ = false;
		Log::instance()->append(CLIENT_MSG_ERROR_WHEN_RECEIVING, Log::ERROR);
		return;
	}

}

dataToSend_t* Client::onEvent() {
	dataToSend_t* data = controller_->handleEvents(&running_);

	int i;
	for (i = 0; name[i] != '\0'; i++)
		data->id[i] = name[i];
	data->id[i] = name[i];
	return data;
}

void Client::onRender(dataFromServer_t data) {

	dataFromClient_t dataToBeDraw;

	dataToBeDraw.cantPersonajes = gameDetails_.cantPersonajes;
	dataToBeDraw.cantObjDinamicos = gameDetails_.cantObjDinamicos;
	dataToBeDraw.cantObjEstaticos = gameDetails_.cantObjEstaticos;
	dataToBeDraw.personajes = data.personajes;
	dataToBeDraw.dinamicos = data.dinamicos;
	dataToBeDraw.estaticos = estaticos_;

	view_->updateView(dataToBeDraw, name);

}

void Client::onCleanup() {
	delete view_;
	delete controller_;
}

bool Client::validateParameters(int argc, char* argv[]) {

	Log::instance()->append(CLIENT_MSG_VAL_PAR, Log::INFO);

	//Validamos cantidad de parametros. En caso de que no sean correctos, se
	//comienza con un juego default
	if (argc != 4) {
		Log::instance()->append(CLIENT_MSG_CANT_PARAM, Log::WARNING);
		help();
		return false;
	}

	Log::instance()->append("Cantidad de parametros correcta", Log::INFO);

	//Leemos el nombre del host
	host = argv[1];
	port = atoi(argv[2]);
	if (port == 0) {
		Log::instance()->append(CLIENT_MSG_INVALID_PORT_ERROR, Log::WARNING);
		return false;
	}
	name = argv[3];

	Log::instance()->append("Parametros correctos!", Log::INFO);

	return CLIENT_OK;
}

/**
 * Metodo para recibir los objetos dinamicos
 */
void Client::recibirDinamicos(figura_t* &dinamicos) {

	int size = sizeof(figura_t) * gameDetails_.cantObjDinamicos;
	dinamicos = (figura_t*) malloc(size);

	recvall(sock, dinamicos, size);
}

/**
 * Metodo para recibir los objetos estaticos
 */
void Client::recibirEstaticos(figura_t* &estaticos) {

	int size = sizeof(figura_t) * gameDetails_.cantObjEstaticos;
	estaticos = (figura_t*) malloc(size);

	recvall(sock, estaticos, size);
}

/**
 * Metodo para recibir los personajes
 */
void Client::recibirPersonajes(personaje_t* &personajes) {

	int size = sizeof(personaje_t) * gameDetails_.cantPersonajes;
	personajes = (personaje_t*) malloc(size);

	recvall(sock, personajes, size);
}

/**
 * Metodo encargado de hacer el envio de informacion
 */
void Client::sendall(int s, void* data, int len) throw (sendException) {
	int total = 0; 			// how many bytes we've sent
	int bytesleft = len; 	// how many we have left to send
	int n;
	while (total < len) {
		n = send(s, data + total, bytesleft, 0);

		//Si aparece un error al enviar, lanzamos una excepcion
		if (n == -1) {
			Log::instance()->append("Error al escribir al servidor",
					Log::ERROR);
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
void Client::recvall(int s, void *data, int len) throw (receiveException) {

	int total = 0;
	int bytesleft = len;
	int n;

	while (total < len) {
		n = recv(s, data + total, bytesleft, 0);

		//Si aparece un error al recibir, lanzamos una excepcion
		if (n == -1 || n == 0) {
			Log::instance()->append("Error al leer del servidor", Log::ERROR);
			throw receiveException();
		}

		total += n;
		bytesleft -= n;
	}

	return;
}

void help() {
	std::cout << "Use mode: ./Client hostname portNro name" << std::endl;
}
