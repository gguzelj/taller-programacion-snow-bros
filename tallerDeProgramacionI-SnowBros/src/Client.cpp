#include "../headers/Client.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Client::Client() {
	this->running_ = true;
	//TODO agregar el struct que va a ser el escenario que reciba del server.
	//model_ = nullptr;
	view_ = nullptr;
	controller_ = nullptr;
	port = 0;
	host = nullptr;
	name = nullptr;
	sock = 0;
	shared_rcv_queue_ = new Threadsafe_queue<receivedData_t*>();

	//Seteamos el nivel del logger:
	//	-Si loggerLevel = INFO se loguean todos los mensajes
	//	-Si loggerLevel = WARNING se loguean solo mensajes de WARNING y ERROR
	//	-Si loggerLevel = ERROR se loguean solo mensajes del tipo ERROR
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

	initialize();
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

int Client::createSocket() {

	Log::instance()->append("Creando Socket", Log::INFO);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		Log::instance()->append("No se pudo crear el socket", Log::ERROR);
		return CLIENT_ERROR;
	}

	Log::instance()->append("Socket creado!", Log::INFO);
	return CLIENT_OK;
}

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

	int size;
	int entro;
	std::string msg;
	firstConnectionDetails_t datos;
	objEstatico_t *objetosEstaticos;
	objDinamico_t *objetosDinamicos;

	//Envio el nombre de cliente
	size = 20;
	sendall(sock, name, &size);

	//Recibo la aprobacion de si entro o no.
	size = sizeof(int);
	recvall(sock, &entro, &size);
	if (entro == SRV_ERROR) {
		Log::instance()->append("El servidor rechazo la conexion", Log::ERROR);
		return CLIENT_ERROR;
	}

	Log::instance()->append("El servidor acepto la conexion", Log::INFO);

	//Recibimos la cantidad de objetos creados en el juego
	size = sizeof(firstConnectionDetails_t);
	if (recvall(sock, &datos, &size) <= 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
		return CLIENT_ERROR;
	}

	msg = "El cliente envio ";
	msg += datos.cantObjDinamicos + " objetos dinamicos y ";
	msg += datos.cantObjEstaticos + " objetos estaticos.";
	Log::instance()->append(msg, Log::INFO);

	//Recibimos la lista de objetos Estaticos
	size = sizeof(objEstatico_t) * datos.cantObjEstaticos;
	objetosEstaticos = (objEstatico_t*) malloc(size);
	if (recvall(sock, objetosEstaticos, &size) <= 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
	}

	//Recibimos la lista de objetos Dinamicos
	size = sizeof(objDinamico_t) * datos.cantObjDinamicos;
	objetosDinamicos = (objDinamico_t*) malloc(size);
	if (recvall(sock, objetosDinamicos, &size) <= 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
	}

	Log::instance()->append("Estos son los objetos Estaticos:", Log::INFO);
	for (unsigned int i = 0; i < datos.cantObjEstaticos; i++) {

		msg = "Alto: ";
		msg += objetosEstaticos[i].alto;
		msg += "/n Ancho: ";
		msg += objetosEstaticos[i].ancho;

		Log::instance()->append(msg, Log::INFO);

	}

	Log::instance()->append("Estos son los objetos Dinamicos:", Log::INFO);
	for (unsigned int i = 0; i < datos.cantObjDinamicos; i++) {

		msg = "Alto: ";
		msg += objetosDinamicos[i].alto;
		msg += "/n Ancho: ";
		msg += objetosDinamicos[i].ancho;

		Log::instance()->append(msg, Log::INFO);

	}
	return CLIENT_OK;
}

void Client::enviarAlServer() {

	int size = sizeof(dataToSend_t);
	dataToSend_t* data = (dataToSend_t*) malloc(size);

	while (running_) {
		//Control all posible events
		onEvent(data, sock, &size);
	}
}

void Client::recibirDelServer() {

	int size = sizeof(receivedData_t);
	receivedData_t* data = (receivedData_t*) malloc(size);

	while (running_) {
		//Voy reciviendo las cosas del servidor
		if (recvall(sock, data, &size) == -1) {
			Log::instance()->append(CLIENT_MSG_ERROR_WHEN_RECEIVING,
					Log::ERROR);
		}
		shared_rcv_queue_->push(data);
	}
}

void Client::onEvent(dataToSend_t* data, int sock, int* size) {
	controller_->handleEvents(&running_, data);
	if (sendall(sock, data, size) == -1) {
		Log::instance()->append(CLIENT_MSG_ERROR_WHEN_SENDING, Log::ERROR);
	}
}

void Client::onRender(receivedData_t* model_) {
	view_->updateView(model_);
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

int Client::recvall(int s, void *data, int *len) {

	int total = 0;                  // how many bytes we've sent
	int bytesleft = *len;   // how many we have left to send
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
	*len = total;                            // return number actually sent here
	return n == -1 || n == 0 ? -1 : 0;     // return -1 on failure, 0 on success
}

int Client::sendall(int s, void *data, int *len) {
	int total = 0;                  // how many bytes we've sent
	int bytesleft = *len;   // how many we have left to send
	int n;
	while (total < *len) {
		n = send(s, data + total, bytesleft, 0);
		if (n == -1) {
			break;
		}
		total += n;
		bytesleft -= n;
	}
	*len = total;                   // return number actually sent here
	return n == -1 ? -1 : 0;               // return -1 on failure, 0 on success
}
