#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Escenario *model, Drawer *view ){
	this->model = model;
	this->view = view;
}
void Controlador::handleEvents(bool* running, bool* reload, SDL_Event* event){

	while(SDL_PollEvent(event)){
		handleEvent(event, running, reload);
	}
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, bool* reload){
	switch (evento->type){
		case SDL_QUIT:
			(*running) = false;
			break;
		case SDL_KEYUP:{
			switch(evento->key.keysym.sym){
				case TECLA_RESET:{
					(*reload) = true;
					(*running) = false;
					break;
				}
				case SDLK_ESCAPE:{
					(*running) = false;
				}
			}
		}
   }
}

