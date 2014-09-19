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
	Log::instance()->append(GAME_MSG_NEW_GAME,Log::INFO);
}

Game::~Game(){
	Log::instance()->append(GAME_MSG_DELETE_GAME,Log::INFO);
	//Log::instance()->closeLog();
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

	if (!(parser_ = new JsonParser(jsonPath))) return false;
	if (parser_->parse()) return false;
	if (!(model_ = new Escenario(parser_))) return false;
	if (!(view_ = new Drawer(parser_))) return false;
	if (!(controller_ = new Controlador(model_, view_))) return false;

	return true;
}

void Game::onEvent(){
	controller_->handleEvents(&running_, &reload_);
}

void Game::onLoop(){
	float32 timeStep = 1/60.0;      //the length of time passed to simulate (seconds)
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position}

	model_->getWorld()->Step( timeStep, velocityIterations, positionIterations);
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
