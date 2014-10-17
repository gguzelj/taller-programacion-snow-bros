#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Drawer *view ){
	this->toggleIzquierdaApretado = false;
	this->toggleDerechaApretado = false;
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
			//aca no habria que usar algun metodo como desconexion o algo asi?
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
				case SDLK_LEFT:{
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					if(!toggleIzquierdaApretado){
					//dataToSend->keycode_= SDLK_LEFT;
					//dataToSend->type_ = SDL_KEYDOWN?;
					}
					break;
				}
				case SDLK_RIGHT:{
					if(!toggleDerechaApretado){
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					//dataToSend->keycode_= SDLK_RIGHT;
					//dataToSend->type_ = SDL_KEYDOWN?;
					}
					break;
				}
				case SDLK_UP:{
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					//dataToSend->keycode_= SDLK_UP;
					//dataToSend->type_ = SDL_KEYDOWN?;
					break;
				}
			}
			break;
		}break;

			case SDL_KEYUP:{
				case SDLK_LEFT:{
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					if(toggleIzquierdaApretado){
					//dataToSend->keycode_= SDLK_LEFT;
					//dataToSend->type_ = SDL_KEYUP?;
					}
					break;
				}
				case SDLK_RIGHT:{
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					if(toggleDerechaApretado){
					//dataToSend->keycode_= SDLK_RIGHT;
					//dataToSend->type_ = SDL_KEYUP?;
					}
					break;
				}
				case SDLK_UP:{
					//TODO enviarle al servidor, o poner en una variable lo que luego se va a enviar al servidor
					//Se pone en una cola?
					//dataToSend->keycode_= SDLK_UP;
					//dataToSend->type_ = SDL_KEYUP?;
					break;
				}
			}break;
		}
}
