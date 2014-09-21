#define VEL_POR_DEFAULT 10.0f
#include "../../../headers/Modelo/Estados/Estados.h"
#include "../../../headers/Modelo/Objetos/Personaje.h"


StandByState Personaje::standby;
JumpingState Personaje::jumping;
WalkingState Personaje::walking;
FallingState Personaje::falling;



//devuelve true si se detuvo o false si no hizo nada
bool detenerMovimientoHorizontal(Personaje* personaje,SDL_Keycode input){

	switch(personaje->getOrientacion()){
		case IZQUIERDA:
			switch(input){
				case SDLK_LEFT:
					personaje->stop();
					Personaje::walking.movimientoLateralIzquierda = false;
					return true;
				case SDLK_RIGHT:
					Personaje::walking.movimientoLateralDerecha = false;
					break;
			}
			break;

		case DERECHA:
			switch(input){
				case SDLK_RIGHT:
					personaje->stop();
					Personaje::walking.movimientoLateralDerecha = false;
					return true;
				case SDLK_LEFT:
					Personaje::walking.movimientoLateralIzquierda = false;
					break;
			}
			break;
        	}
        return false;
}



void OnAirState::handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type){
        switch(input_type){
                case SDL_KEYDOWN:

                        switch(input){
                                case SDLK_LEFT:
                                        personaje.moveLeft();
                                        personaje.setOrientacion(IZQUIERDA);
                                        break;

                                case SDLK_RIGHT:
                                        personaje.moveRight();
                                        personaje.setOrientacion(DERECHA);
                                        break;
                        }
                        break;


                case SDL_KEYUP:
                        detenerMovimientoHorizontal(&personaje,input);
                        break;

        }

}




void StandByState::handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type){
        switch(input_type){
                case SDL_KEYDOWN:{
                        switch(input){

                                        case SDLK_UP:{
                                                personaje.state = &Personaje::jumping;
                                                personaje.jump();
                                                break;
                                        }

                                        case SDLK_LEFT:{
                                        		Personaje::walking.movimientoLateralIzquierda = true;
                                                personaje.state = &Personaje::walking;
                                                personaje.moveLeft();
                                                personaje.setOrientacion(IZQUIERDA);
                                                break;
                                        }

                                        case SDLK_RIGHT:{
                                        		Personaje::walking.movimientoLateralDerecha = true;
                                                personaje.state = &Personaje::walking;
                                                personaje.moveRight();
                                                personaje.setOrientacion(DERECHA);
                                                break;
                                        }
                        }
                }

        }
}


void WalkingState::caminar(Personaje &personaje){
	 switch(personaje.getOrientacion()){
	                        case IZQUIERDA:{
	                                personaje.moveLeft();
	                                break;
	                        }
	                        case DERECHA:{
	                                personaje.moveRight();
	                                break;
	                        }
	        }
}






void WalkingState::handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type){

	switch(input_type){
		case SDL_KEYDOWN:
			switch(input){
				case SDLK_UP:
					personaje.state = &Personaje::jumping;
					personaje.jump();
					break;
				case SDLK_LEFT:
					Personaje::walking.movimientoLateralIzquierda = true;
					break;
				case SDLK_RIGHT:
					Personaje::walking.movimientoLateralDerecha =true;
					break;

				}
			break;

		case SDL_KEYUP:
			detenerMovimientoHorizontal(&personaje,input);
			if ( Personaje::walking.movimientoLateralDerecha == false && Personaje::walking.movimientoLateralIzquierda == false)
				personaje.state = &Personaje::standby;
			else{
				switch(personaje.getOrientacion()){
					case IZQUIERDA:
						if(Personaje::walking.movimientoLateralIzquierda == false)
							personaje.setOrientacion(DERECHA);
					case DERECHA:
						if(Personaje::walking.movimientoLateralDerecha == false)
							personaje.setOrientacion(IZQUIERDA);
				}
			}
			break;
	}
}

