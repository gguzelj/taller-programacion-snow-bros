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
	port = 0;
	sock = 0;
	shared_rcv_queue_ = new Threadsafe_queue<dataFromServer_t>();

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

	if(initialize() == CLIENT_ERROR)
		return CLIENT_ERROR;

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

	//Envio el nombre de cliente
	size = 20;
	sendall(sock, name, &size);

	//Recibo la aprobacion de si entro o no.
	size = sizeof(int);
	if (recvall(sock, &entro, &size) != 0) {
		Log::instance()->append("Error al escribir al servidor", Log::ERROR);
		return CLIENT_ERROR;
	}

	if (entro == SRV_ERROR) {
		Log::instance()->append("El servidor rechazo la conexion", Log::ERROR);
		return CLIENT_ERROR;
	}

	Log::instance()->append("El servidor acepto la conexion", Log::INFO);


	//Recibimos la cantidad de objetos creados en el juego
	size = sizeof(firstConnectionDetails_t);
	if (recvall(sock, &gameDetails_, &size) != 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
		return CLIENT_ERROR;
	}

	Log::instance()->append("Recibimos los objetos estaticos.", Log::INFO);

	size = sizeof(figura_t) * gameDetails_.cantObjEstaticos;
	estaticos_ = (figura_t*) malloc(size);
	if (recvall(sock, estaticos_, &size) != 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
		return CLIENT_ERROR;
	}

	Log::instance()->append("Recibimos los objetos dinamicos.", Log::INFO);

	size = sizeof(figura_t) * gameDetails_.cantObjDinamicos;
	dinamicos_ = (figura_t*) malloc(size);
	if (recvall(sock, dinamicos_, &size) != 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
		return CLIENT_ERROR;
	}

	size = sizeof(personaje_t) * gameDetails_.cantPersonajes;
	personajes_ = (personaje_t*) malloc(size);
	if (recvall(sock, personajes_, &size) != 0) {
		Log::instance()->append("No se pueden recibir datos", Log::WARNING);
		return CLIENT_ERROR;
	}

	Log::instance()->append("Recibimos los personajes.", Log::INFO);

	personaje_t personajePrincipal;
	for(int i = 0; i < gameDetails_.cantPersonajes;i++ ){
		if(strcmp( (personajes_[i]).id,name) == 0)
			personajePrincipal = personajes_[i];
	}
	view_->inicializarCamara(personajePrincipal);

    std::cout << "Recibimos " << gameDetails_.cantObjDinamicos << " obj Dinamicos y ";
    std::cout << gameDetails_.cantObjEstaticos << " obj Estaticos" << std::endl;
    std::cout <<"recibimos"<< gameDetails_.cantPersonajes << "personajes"<< std::endl;

	return CLIENT_OK;
}

void Client::enviarAlServer() {
	FILE* file = fopen ("clSend.txt","w");
	fclose(file);

	int size = sizeof(dataToSend_t);
	while (running_) {
		//Control all posible events
		dataToSend_t* data = onEvent();
		if (data->keycode_1 == 0 && data->keycode_2 ==0 ){
			free(data);
			continue;
		}
		if (sendall(sock, data, &size) == -1) {
			Log::instance()->append(CLIENT_MSG_ERROR_WHEN_SENDING, Log::ERROR);
		}
		FILE* file = fopen ("clSend.txt","a");
		fprintf(file, "Cliente: %s\ntype_1: %d\nkeycode_1: %d\ntype_2: %d\nkeycode_2: %d\n",data->id,data->type_1,data->keycode_1,data->type_2,data->keycode_2);
		fclose(file);
		SDL_Delay(1);
		free(data);
	}
}

void Client::recibirDelServer() {

	int size;
	dataFromServer_t data;

	FILE* file = fopen ("clRecv.txt","w");
	fclose(file);

	//La cant de bytes a recibir esta definida por la cantidad de
	//personajes y la cantidad de objetos dinamicos:
	while (running_) {

		//Recibimos los personajes
		size = sizeof(personaje_t) * gameDetails_.cantPersonajes;
		data.personajes = (personaje_t*) malloc(size);
		if (recvall(sock, data.personajes, &size) != 0) {
			//Log::instance()->append(CLIENT_MSG_ERROR_WHEN_RECEIVING, Log::ERROR);
		}

		//Recibimos los objetos dinamicos
		size = sizeof(figura_t) * gameDetails_.cantObjDinamicos;
		data.dinamicos = (figura_t*) malloc(size);
		if (recvall(sock, data.dinamicos, &size) != 0) {
			//Log::instance()->append(CLIENT_MSG_ERROR_WHEN_RECEIVING, Log::ERROR);
		}
		shared_rcv_queue_->push(data);

		file = fopen("clRecv.txt", "a");
		fprintf(file, "Cant Personajes: %d\nCant Dinamicos: %d\n", gameDetails_.cantObjDinamicos, gameDetails_.cantPersonajes);
		fclose(file);
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

	view_->updateView(dataToBeDraw,name);

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
 * Metodo encargado de hacer el envio de informacion
 */
int Client::sendall(int s, void* data, int* len) {
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
int Client::recvall(int s, void *data, int *len) {

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

void help(){
	std::cout << "Use mode: ./Client hostname portNro name" << std::endl;
}
