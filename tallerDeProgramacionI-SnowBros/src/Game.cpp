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

	if (!(parser_ = new JsonParser(jsonPath))) return false;
	if (!(model_ = new Escenario(parser_))) return false;
	if (!(view_ = new Drawer(parser_))) return false;
	if (!(controller_ = new Controlador(model_, view_))) return false;

	return true;
}

void Game::onEvent(){
	controller_->handleEvents(&running_, &reload_);
}

void Game::onLoop(){
//Game step
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
