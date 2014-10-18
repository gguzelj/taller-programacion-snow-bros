#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Drawer *view ){
	this->view = view;
	cantEventos = 0;
}
void Controlador::handleEvents(bool* running, dataToSend_t* data){
	SDL_Event event;
	cantEventos = 0;
	/*
	 * Como puede haber mucha cantidad de eventos, pero solo nos importan dos
	 * de las flechas, cuando tomo dos eventos de flechas, si llega a haber mas
	 * eventos de este tipo (cosa que dudo, pero por las dudas) se ignoran.
	 * No pongo un break directamente porque puede haber mas eventos que no sean
	 * para enviar al server.
	 */
	while(SDL_PollEvent(&event)){
		if(cantEventos == 0)
			handleEvent(&event, running, &(data->keycode_1),&(data->type_1));
		if(cantEventos == 1)
			handleEvent(&event, running, &(data->keycode_2),&(data->type_2));
		if(cantEventos == 2){
			int* aux1;
			unsigned int* aux2;
			handleEvent(&event, running, aux1, aux2);
		}
	}
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, int *code, unsigned int *type){
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
					*type = SDL_KEYDOWN;
					*code= SDLK_LEFT;
					cantEventos++;
					break;
				}
				case SDLK_RIGHT:{
					*type = SDL_KEYDOWN;
					*code= SDLK_RIGHT;
					cantEventos++;
					break;
				}
				case SDLK_UP:{
					*type = SDL_KEYDOWN;
					*code= SDLK_UP;
					cantEventos++;
					break;
				}
			}
			break;
		}break;

			case SDL_KEYUP:{
				case SDLK_LEFT:{
					*type = SDL_KEYUP;
					*code= SDLK_LEFT;
					cantEventos++;
					break;
				}
				case SDLK_RIGHT:{
					*type = SDL_KEYUP;
					*code= SDLK_RIGHT;
					cantEventos++;
					break;
				}
				case SDLK_UP:{
					*type = SDL_KEYUP;
					*code= SDLK_UP;
					cantEventos++;
					break;
				}
			}break;
		}
}
