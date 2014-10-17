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
	port = nullptr;
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

	while(running_){
		//Control all posible events
		onEvent();

		//Update the view
		onRender();

		SDL_Delay(5);
	}

	//cleaning up
	onCleanup();

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

void Client::onEvent(){
	controller_->handleEvents(&running_);
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
/**
 * Validamos los parametros recibidos por consola. En caso de que el archivo JSON
 * no exista, seteamos uno por default
 */
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
