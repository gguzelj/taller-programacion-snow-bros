#include "../headers/Game.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Game::Game(){
	this->running = true;
	this->reload = false;
}

int Game::onExecute(string jsonPath){

	if(onInit(jsonPath) == false){
		return 1; //Exit with error
	}

	while(running){

		//Control all posible events
		onEvent();

		//Update the model
		onLoop();

		//Update the view
		onRender();
	}

	//cleaning up
	onCleanup();

	if (reload) return 2; //Reload the game

	return 0; //Exit with everything ok
}

bool Game::onInit(string jsonPath){

	parser = new JsonParser(jsonPath);

	model = new Escenario(parser);
	view = new Drawer(parser, model); //La vista conoce al modelo ?
	controller = new Controlador(model, view);

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
	controller->handleEvents(&running, &reload);
}

void Game::onLoop(){
//Game step
}

void Game::onRender(){
	//drawer->updateView(world);
}

void Game::onCleanup(){
	delete view;
	delete model;
	delete controller;
    delete parser;
}

// ########################## //
// ##### Private methods #### //
// ########################## //
