#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Escenario *model, Drawer *view ){
	this->model = model;
	this->view = view;
	presionando = 0;
}
void Controlador::handleEvents(bool* running, bool* reload){

	SDL_Event event;

	while(SDL_PollEvent(&event)){
		handleEvent(&event, running, reload);
	}
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, bool* reload){
	switch (evento->type){
		case SDL_QUIT:
			(*running) = false;
			break;
		case SDL_KEYDOWN:{
			switch(evento->key.keysym.sym){
				case SDLK_UP:{
					model->getPersonaje()->jump();
					break;
				}
			}
			if (presionando == 0){

			switch(evento->key.keysym.sym){
					case TECLA_RESET:{
						(*reload) = true;
						(*running) = false;
						break;
					}
					case SDLK_ESCAPE:{
						(*running) = false;
						break;
					}
					case SDLK_LEFT:{
						model->getPersonaje()->moveLeft();
						presionando = 1;
						movimientoTipo = 0;
						break;
					}
					case SDLK_RIGHT:{
						model->getPersonaje()->moveRight();
						presionando = 1;
						movimientoTipo = 1;
						break;
					}
				}
			}
			else{
				if (movimientoTipo==0)
					model->getPersonaje()->moveLeft();
				else 	model->getPersonaje()->moveRight();
			}
			break;
		}
		case SDL_KEYUP:{
			switch(evento->key.keysym.sym){
				case SDLK_LEFT:{
					model->getPersonaje()->stop();
					presionando = 0;
					break;
				}
				case SDLK_RIGHT:{
					model->getPersonaje()->stop();
					presionando = 0;
					break;
				}
			}
		}
   }
}

