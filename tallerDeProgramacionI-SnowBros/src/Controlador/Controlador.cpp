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

	while(SDL_PollEvent(&event) && running){
		if(cantEventos == 0)
			handleEvent(&event, running, &(data->keycode_1),&(data->type_1));
		if(cantEventos == 1)
			handleEvent(&event, running, &(data->keycode_2),&(data->type_2));
		if(cantEventos == 2)
			handleEvent(&event, running, &(data->keycode_3),&(data->type_3));
		if(cantEventos == 3)
			handleEvent(&event, running, &(data->keycode_4),&(data->type_4));
		if(cantEventos == 4){
			int aux1 = 0;
			unsigned int aux2 = 0;
			handleEvent(&event, running, &aux1, &aux2);
		}
	}
	return data;
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, int32_t *code, Uint32 *type){
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
				case SDLK_m:{
					//If the music is paused
					if( Mix_PausedMusic() == 1 )
						Mix_ResumeMusic();	//Resume the music
					//If the music is playing
					else
						Mix_PauseMusic(); 	//Pause the music
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
				case SDLK_RCTRL:{
					*type = SDL_KEYDOWN;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_SPACE:{
					*type = SDL_KEYDOWN;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
			}
			break;
		}

		case SDL_KEYUP:{
			switch(evento->key.keysym.sym){
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
				case SDLK_SPACE:{
					*type = SDL_KEYUP;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
				case SDLK_RCTRL:{
					*type = SDL_KEYUP;
					*code= evento->key.keysym.sym;
					cantEventos++;
					break;
				}
			}break;
		}
	}
}
