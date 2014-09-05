/*
 * GestorDeEventos.cpp
 *
 *  Created on: 4/9/2014
 *      Author: anonimo
 */

#include "../headers/GestorDeEventos.h"


GestorDeEventos::GestorDeEventos() {
	// TODO Auto-generated constructor stub

}

GestorDeEventos::~GestorDeEventos() {
	// TODO Auto-generated destructor stub
}


void GestorDeEventos::interpretarEvento(SDL_Event evento,bool* quit, bool* reload){
	switch (evento.type){
	       case SDL_QUIT:
	        	(*quit) = true;
	        	break;


	       case SDL_KEYUP:{
	        	switch(evento.key.keysym.sym){
	        		case TECLA_RESET:{
	        		(*reload) = true;
	        		(*quit) = true;
	        		break;


	        		}
	        	}
	        }
	}
}
