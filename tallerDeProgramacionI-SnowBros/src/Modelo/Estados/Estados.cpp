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
                                                                Personaje::walking.movimientoLateral = false;
                                                                return true;
                                                        }
                                        break;

                                        case DERECHA:
                                                        switch(input){
                                                                case SDLK_RIGHT:
                                                                        personaje->stop();
                                                                        Personaje::walking.movimientoLateral = false;
                                                                        return true;
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
                                                personaje.state = &Personaje::walking;
                                                personaje.moveLeft();
                                                personaje.setOrientacion(IZQUIERDA);
                                                break;
                                        }

                                        case SDLK_RIGHT:{
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
                                                Personaje::walking.movimientoLateral = true;
                                                break;
                        }
                        break;

                case SDL_KEYUP:
                        if(detenerMovimientoHorizontal(&personaje,input))
                                personaje.state = &Personaje::standby;
                        break;
        }
}

