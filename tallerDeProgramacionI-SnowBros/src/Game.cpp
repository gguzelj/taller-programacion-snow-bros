#include "../headers/Game.h"

// ########################## //
// ##### Public methods ##### //
// ########################## //

Game::Game(){
	this->running = true;
	this->reload = false;
	this->drawer = new Drawer();
	this->eventHandler = new GestorDeEventos();
	this->dataHandler = new Controlador();
	this->world = initModel();
}

int Game::onExecute(string jsonPath){

	if(onInit(jsonPath) == false){
		return 1; //Exit with error
	}

	SDL_Event event;
	while(running){

		//Control all posible events
		onEvent(&event);

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

	//Inicializa el modelo con los objetos requeridos por el JSON
	if ((dataHandler->setWorldObjects(jsonPath,world)) == false) return false;

	//Inicializa la ventana para mostrar el modelo, de acuerdo a los parametros requeridos por el JSON
	if((dataHandler->initWindow(jsonPath,drawer)) == false) return false;

	return true;
}

void Game::onEvent(SDL_Event* event){
	eventHandler->listenEvents(event, &running, &reload);
}

void Game::onLoop(){

}

void Game::onRender(){
	drawer->updateView(world);
}

void Game::onCleanup(){
	delete drawer;
	delete eventHandler;
	delete dataHandler;
    delete world;
}

// ########################## //
// ##### Private methods #### //
// ########################## //

b2World* Game::initModel(){
	ObjectFactory* objFactory = new ObjectFactory();
	b2World* model = objFactory->crearb2World();
	delete objFactory;
	return model;
}
