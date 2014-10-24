#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Drawer *view ){
	this->view = view;
	cantEventos = 0;
}
dataToSend_t* Controlador::handleEvents(bool* running){
	SDL_Event event;
	cantEventos = 0;

	int size = sizeof(dataToSend_t);
	dataToSend_t* data = (dataToSend_t*) malloc(size);
	bzero(data, size);

	while(SDL_PollEvent(&event) && &running){
		if(cantEventos == 0)
			handleEvent(&event, running, &(data->keycode_1),&(data->type_1));
		if(cantEventos == 1)
			handleEvent(&event, running, &(data->keycode_2),&(data->type_2));
		if(cantEventos == 2){
			int aux1 = 0;
			unsigned int aux2 = 0;
			handleEvent(&event, running, &aux1, &aux2);
		}
	}
	return data;
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, int32_t *code, Uint32 *type){
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
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_RIGHT:{
					*type = SDL_KEYDOWN;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_UP:{
					*type = SDL_KEYDOWN;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
			}
			break;
		}break;

			case SDL_KEYUP:{
				case SDLK_LEFT:{
					*type = SDL_KEYUP;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_RIGHT:{
					*type = SDL_KEYUP;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_UP:{
					*type = SDL_KEYUP;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
			}break;
		}
}
