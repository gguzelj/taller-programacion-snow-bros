#include "../headers/GestorDeEventos.h"

GestorDeEventos::GestorDeEventos() {
}

GestorDeEventos::~GestorDeEventos() {
}

void GestorDeEventos::listenEvents(SDL_Event* event, bool* running, bool* reload){
	while(SDL_PollEvent(event)){
		interpretarEvento(event, running, reload);
	}
}

void GestorDeEventos::interpretarEvento(SDL_Event* evento,bool* running, bool* reload){
	switch (evento->type){
		case SDL_QUIT:
			(*running) = true;
			break;
		case SDL_KEYUP:{
			switch(evento->key.keysym.sym){
				case TECLA_RESET:{
					(*reload) = true;
					(*running) = true;
					break;
					}
				}
		}
   }
}
