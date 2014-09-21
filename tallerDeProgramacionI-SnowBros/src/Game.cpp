#include "../headers/Game.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Game::Game(){
	this->running_ = true;
	this->reload_ = false;
	model_ = nullptr;
	view_ = nullptr;
	controller_ = nullptr;
	parser_ = nullptr;

	//Seteamos el nivel del logger:
	//	-Si loggerLevel = INFO se loguean todos los mensajes
	//	-Si loggerLevel = WARNING se loguean solo mensajes de WARNING y ERROR
	//	-Si loggerLevel = ERROR se loguean solo mensajes del tipo ERROR
	Log::instance()->loggerLevel = Log::INFO;
	Log::instance()->append(GAME_MSG_NEW_GAME,Log::INFO);
}

Game::~Game(){
	Log::instance()->append(GAME_MSG_DELETE_GAME,Log::INFO);
	Log::instance()->closeLog();
}

int Game::onExecute(int argc, char* argv[]){

	validateParameters(argc,argv);

	if(onInit(jsonPath_) == false){
		Log::instance()->append(GAME_MSG_INIT_ERROR,Log::INFO);
		return GAME_ERROR;
	}

	while(running_){
		//Control all posible events
		onEvent();

		//Update the model
		onLoop();

		//Update the view
		onRender();

		SDL_Delay(1);
	}

	//cleaning up
	onCleanup();

	if (reload_){
		Log::instance()->append(GAME_MSG_RELOAD,Log::INFO);
		return GAME_RELOAD;
	}

	return GAME_OK;
}

bool Game::onInit(string jsonPath){

	try{
		parser_ = new JsonParser(jsonPath);
		if (parser_->parse()) return false;
		model_ = new Escenario(parser_);
		view_ = new Drawer(parser_);
		controller_ = new Controlador(model_, view_);

		return true;
	}
	catch(exception& e){
		std::cout<<e.what()<<std::endl;
		return false;
	}
}
void Game::onEvent(){
	controller_->handleEvents(&running_, &reload_);
}

void Game::onLoop(){

	//chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
	//esta implementado aca para que cambie cuando tiene que hacerlo
	if(model_->getPersonaje()->getVelocity().y <= 0.0f && model_->getPersonaje()->getCantidadDeContactosActuales() == 0 ){
		model_->getPersonaje()->state = &Personaje::falling;
	} else if (model_->getPersonaje()->getVelocity().y <= 0.0f && model_->getPersonaje()->state == &Personaje::jumping ){
		model_->getPersonaje()->state = &Personaje::standby;
	}

	if(Personaje::walking.movimientoLateralDerecha || Personaje::walking.movimientoLateralIzquierda)
		Personaje::walking.caminar(*(model_->getPersonaje()));

	if(Personaje::jumping.debeSaltar && model_->getPersonaje()->state->getCode() != JUMPING && model_->getPersonaje()->state->getCode() != FALLING){
		model_->getPersonaje()->jump();
		model_->getPersonaje()->state = &Personaje::jumping;
	}

	model_->getWorld()->Step(timeStep, velocityIterations, positionIterations);
}
void Game::onRender(){
	view_->updateView(model_);
}

void Game::onCleanup(){
	delete view_;
	delete model_;
	delete controller_;
    delete parser_;
}

// ########################## //
// ##### Private methods #### //
// ########################## //
/**
 * Validamos los parametros recibidos por consola. En caso de que el archivo JSON
 * no exista, seteamos uno por default
 */
void Game::validateParameters(int argc, char* argv[]){

	Log::instance()->append(GAME_MSG_VAL_PAR,Log::INFO);

	//Leemos el path del archivo json
	if(argc >= 2) jsonPath_ = argv[1];

	//Validamos cantidad de parametros. En caso de que no sean correctos, se
	//comienza con un juego default
	if(argc != 2){
		Log::instance()->append(GAME_MSG_CANT_PARAM,Log::WARNING);
		jsonPath_ = GAME_JSON_DEF;
	}

	//Validamos que realmente exista el archivo json
	std::ifstream file(jsonPath_);
	if(!file.good()){
		Log::instance()->append(GAME_MSG_JSON_FILE, Log::WARNING);
		jsonPath_ = GAME_JSON_DEF;
	}

	return;
}
