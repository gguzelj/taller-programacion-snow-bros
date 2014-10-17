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

int Client::run(){

	//TODO crear el thread para enviar al server y el thread para recibir.

	//Main thread
	while(running_){

		//Habria que agregar el recibir del servidor que cosas cambiaron?


		//Mientras este corriendo quiero enviarle mis imputs
		//enviarAlServer(sock);	//que socket se le enviaria?

		//Update the view
		onRender();

		SDL_Delay(5);
	}

	//cleaning up
	onCleanup(); //barre que te barre

	return CLIENT_OK;
}

bool Client::init(int argc, char* argv[]){

	if(validateParameters(argc,argv) == CLIENT_ERROR)
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

void Client::enviarAlServer(int sock){

	int size = sizeof(dataToSend_t);
	dataToSend_t* data = (dataToSend_t*) malloc(size);

	while(running_){
		//Control all posible events
		/* TODO Sudden realization. Se hacen varios SDL_PollEvent
		 * al tomar los eventos. Osea que en cada step hay mas un evento simultaneo,
		 * bah, puede haber mas de uno. Voy ahondar mas en el tema y ver si toman
		 * hasta un limite (esto es lo que yo decia de que si tocas mas de una tecla
		 * te toma las dos, pero me parece que tres ya no toma).
		 */
		onEvent(&(data->keycode_),&(data->type_));
		sendall(size,data, &size);	//el primer o el ultimo size esta mal, a menos que...
	}
}

void Client::onEvent(int *code, unsigned int *type){
	controller_->handleEvents(&running_, code, type);
}

void Client::onRender(){
	//TODO lo mismo de antes pero lo remarco. Agregar el struct del escenario.
	//view_->updateView(model_);
}

void Client::onCleanup(){
	delete view_;
	delete controller_;
}

// ########################## //
// ##### Private methods #### //
// ########################## //

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

int Client::recvall(int s, receivedData_t *data, int *len) {

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

int Client::sendall(int s, dataToSend_t *data, int *len) {
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
