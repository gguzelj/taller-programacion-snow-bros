#define VEL_POR_DEFAULT 10.0f
#include "../../../headers/Modelo/Estados/Estados.h"
#include "../../../headers/Modelo/Objetos/Personaje.h"


StandByState Personaje::standby;
JumpingState Personaje::jumping;
WalkingState Personaje::walking;
FallingState Personaje::falling;



void cambiarOrientacionAlDejarDePresionarUnaTecla(Personaje &personaje){
	switch(personaje.getOrientacion()){
		case IZQUIERDA:
			if(Personaje::walking.movimientoLateralIzquierda == false)
				personaje.setOrientacion(DERECHA);
			break;
		case DERECHA:
			if(Personaje::walking.movimientoLateralDerecha == false)
				personaje.setOrientacion(IZQUIERDA);
			break;
	}
}




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
                                		if(!Personaje::walking.movimientoLateralDerecha){
                                			personaje.moveLeft();
                                        	personaje.setOrientacion(IZQUIERDA);
                                		}
                                        Personaje::walking.movimientoLateralIzquierda = true;
                                        break;

                                case SDLK_RIGHT:
                                		if(!Personaje::walking.movimientoLateralIzquierda){
                                			personaje.moveRight();
                                        	personaje.setOrientacion(DERECHA);
                                		}
                                        Personaje::walking.movimientoLateralDerecha = true;
                                        break;
                        }
                        break;


                case SDL_KEYUP:
                		if(input == SDLK_UP)
                			Personaje::jumping.debeSaltar = false;
                        detenerMovimientoHorizontal(&personaje,input);
                        if(Personaje::walking.movimientoLateralIzquierda || Personaje::walking.movimientoLateralDerecha)
                        	cambiarOrientacionAlDejarDePresionarUnaTecla(personaje);
                         break;

        }

}


void saltar(Personaje& personaje){
	if (personaje.getJumpCooldown() <=0){
		Personaje::jumping.debeSaltar = true;
		personaje.state = &Personaje::jumping;
		personaje.jump();
	}
}


void StandByState::handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type){
        switch(input_type){
                case SDL_KEYDOWN:{
                        switch(input){

                                        case SDLK_UP:{
                                        		saltar(personaje);
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
                break;

                case SDL_KEYUP:
                	if(input == SDLK_UP)
                	   Personaje::jumping.debeSaltar = false;

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
					saltar(personaje);
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
			if(input == SDLK_UP)
			      Personaje::jumping.debeSaltar = false;
			detenerMovimientoHorizontal(&personaje,input);
			if ( Personaje::walking.movimientoLateralDerecha == false && Personaje::walking.movimientoLateralIzquierda == false)
				personaje.state = &Personaje::standby;
			else
				cambiarOrientacionAlDejarDePresionarUnaTecla(personaje);
			break;
	}
}

