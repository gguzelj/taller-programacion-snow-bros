#include "../headers/Game.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Game::Game(){
	this->running_ = true;
	this->reload_ = false;
}

int Game::onExecute(string jsonPath){

	if(onInit(jsonPath) == false){
		return 1; //Exit with error
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

	if (reload_) return 2; //Reload the game

	return 0; //Exit with everything ok
}

bool Game::onInit(string jsonPath){

	parser_ = new JsonParser(jsonPath);

	model_ = new Escenario(parser_);
	view_ = new Drawer(parser_, model_); //La vista conoce al modelo ?
	controller_ = new Controlador(model_, view_);

	/*
	 * Dentro de los constructores de la vista y el modelo deberian existir llamados a
	 * metodos definidos en el parser, como por ejemplo:
	 *
	 * List<b2BodyDef> objetos = parser->getObjetos();
	 * loop objetos into object
	 * 		world->createBody();
	 *
	 */
	return true;
}

void Game::onEvent(){
	controller_->handleEvents(&running_, &reload_);
}

void Game::onLoop(){
//Game step
}

void Game::onRender(){
	//drawer->updateView(world);
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
