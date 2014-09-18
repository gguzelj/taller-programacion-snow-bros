#include "../../headers/Controlador/Controlador.h"

Controlador::Controlador(Escenario *model, Drawer *view ){
	this->model = model;
	this->view = view;
	presionando = false;
}
void Controlador::handleEvents(bool* running, bool* reload){

	SDL_Event event;

	while(SDL_PollEvent(&event)){
		handleEvent(&event, running, reload);
	}
}

void Controlador::handleEvent(SDL_Event* evento,bool* running, bool* reload){
	if (presionando == true){
		if (verdaderoSiDerechaFalsoSiIzquierda==false)
		model->getPersonaje()->moveLeft();
		else 	model->getPersonaje()->moveRight();
	};
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
								if (presionando == false){
									presionando = true;
									verdaderoSiDerechaFalsoSiIzquierda = false;
									model->getPersonaje()->moveLeft();
								};
								break;
							}
							case SDLK_RIGHT:{
								if (presionando == false){
									presionando = true;
									verdaderoSiDerechaFalsoSiIzquierda = true;
									model->getPersonaje()->moveRight();
								};
								break;
							}

						}
						break;
					}

					if (presionando == true){
						if (verdaderoSiDerechaFalsoSiIzquierda==false)
						model->getPersonaje()->moveLeft();
						else 	model->getPersonaje()->moveRight();
						case SDL_KEYUP:{
							switch(evento->key.keysym.sym){
								case SDLK_LEFT:{
									if (verdaderoSiDerechaFalsoSiIzquierda == false){ //si va a derecha no lo dejo frenarse
										model->getPersonaje()->stop();
										presionando = false;
									};
									break;
								}
								case SDLK_RIGHT:{
									if (verdaderoSiDerechaFalsoSiIzquierda == true){ //si va a izquierda no lo dejo frenarse
										model->getPersonaje()->stop();
										presionando = false;
									};
									break;
								}
							}
						}
					};
	}
		if (presionando == true){
			if (verdaderoSiDerechaFalsoSiIzquierda==false)
			model->getPersonaje()->moveLeft();
			else 	model->getPersonaje()->moveRight();
		};
}
