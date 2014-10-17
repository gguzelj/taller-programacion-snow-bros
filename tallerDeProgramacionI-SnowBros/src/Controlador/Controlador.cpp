#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Drawer *view ){
	this->view = view;
}
void Controlador::handleEvents(bool* running, int *code, unsigned int *type){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		handleEvent(&event, running);
	}
}

void Controlador::handleEvent(SDL_Event* evento,bool* running){
	//logica de manejo de eventos del personaje
	//model->getPersonaje()->handleInput(evento->key.keysym.sym,evento->type);

	switch (evento->type){
		case SDL_QUIT:
			(*running) = false;
			break;

		case SDL_KEYDOWN:{
			switch(evento->key.keysym.sym){
				case SDLK_ESCAPE:{
					(*running) = false;
					break;
				}
				case SDLK_PLUS:{
					view->zoomIn();
					break;
				}
				case SDLK_KP_PLUS:{
					view->zoomIn();
					break;
				}
				case SDLK_MINUS:{
					view->zoomOut();
					break;
				}
				case SDLK_KP_MINUS:{
					view->zoomOut();
					break;
				}
			}
			break;
		}
	}
}
