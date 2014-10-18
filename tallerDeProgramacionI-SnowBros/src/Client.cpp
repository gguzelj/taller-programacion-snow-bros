#include "../headers/Client.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Client::Client(){
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
	Log::instance()->append(CLIENT_MSG_NEW_CLIENT,Log::INFO);
}

Client::~Client(){
	Log::instance()->append(CLIENT_MSG_DELETE_CLIENT,Log::INFO);
	Log::instance()->closeLog();
}

/*
 * Inicializamos el cliente
 */
bool Client::init(int argc, char* argv[]){

	if(validateParameters(argc,argv) == CLIENT_ERROR)
		return CLIENT_ERROR;

	if(createSocket() == CLIENT_ERROR)
		return CLIENT_ERROR;

	try{
		view_ = new Drawer();
		controller_ = new Controlador(view_);
		//TODO cambiar el modelo por lo que reciba del server
		//view_->inicializarCamara(model_->getPersonaje());
		return true;
	}
	catch(exception& e){
		std::cout<<e.what()<<std::endl;
		Log::instance()->append(CLIENT_MSG_INIT_ERROR,Log::INFO);
		return CLIENT_ERROR;
	}
}

int Client::run(){

	if(connectToServer() == CLIENT_ERROR)
		return CLIENT_ERROR;

	initialize();
	sendTh = std::thread(&Client::enviarAlServer, this);
	recvTh = std::thread(&Client::recibirDelServer, this);

	//Main thread
	while(running_){

		//Update the view
		onRender();

		SDL_Delay(1);
	}

	//cleaning up
	onCleanup();

	return CLIENT_OK;
}

// ########################## //
// ##### Private methods #### //
// ########################## //

int Client::createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        return CLIENT_ERROR;
	}
    return CLIENT_OK;
}

int Client::connectToServer(){
    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname(host);
    if (server == NULL) {
        return CLIENT_ERROR;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        return CLIENT_ERROR;
	}
    return CLIENT_OK;
}

int Client::initialize(){

	int sizeOfName = 20;
	int entro;
	int intSize = sizeof(int);

	//Envio el nombre de cliente
	sendall(sock, name, &sizeOfName);

	//Recibo la aprobacion de si entro o no.
	recvall(sock, &entro, &intSize);
	if(!entro)
		return CLIENT_ERROR;

	//TODO recibir la informacion inicial del servidor.

	return CLIENT_OK;
}

void Client::enviarAlServer(){

	int size = sizeof(dataToSend_t);
	dataToSend_t* data = (dataToSend_t*) malloc(size);

	while(running_){
		//Control all posible events
		onEvent(data);
		sendall(sock,data, &size);
	}
}

void Client::recibirDelServer(){

}

void Client::onEvent(dataToSend_t* data){
	controller_->handleEvents(&running_, data);
}

void Client::onRender(){
	//TODO lo mismo de antes pero lo remarco. Agregar el struct del escenario.
	//view_->updateView(model_);
}

void Client::onCleanup(){
	delete view_;
	delete controller_;
}

bool Client::validateParameters(int argc, char* argv[]){

	Log::instance()->append(CLIENT_MSG_VAL_PAR,Log::INFO);

	//Validamos cantidad de parametros. En caso de que no sean correctos, se
	//comienza con un juego default
	if(argc != 4){
		Log::instance()->append(CLIENT_MSG_CANT_PARAM,Log::WARNING);
		return false;
	}

	//Leemos el nombre del host
	host = argv[1];
	port = atoi(argv[2]);
	if(port == 0){
		Log::instance()->append(CLIENT_MSG_INVALID_PORT_ERROR,Log::WARNING);
		return false;
	}
	name = argv[3];

	return true;
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
        *len = total;                                   // return number actually sent here
        return n == -1 || n == 0 ? -1 : 0;      // return -1 on failure, 0 on success
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
        return n == -1 ? -1 : 0;                // return -1 on failure, 0 on success
}
