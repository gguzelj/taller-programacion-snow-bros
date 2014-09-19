/*
 * Estados.h
 *
 *  Created on: 18/9/2014
 *      Author: anonimo
 */

#ifndef ESTADOS_H_
#define ESTADOS_H_
#include "Box2D/Box2D.h"
#include "PersonajeState.h"
#define JUMPING 'j'
#define STANDBY 's'
#define WALKING 'w'
#define FALLING 'f'


class OnAirState: public PersonajeState{
public:
	void handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type);
};



class JumpingState: public OnAirState{
public:
	char getCode(){
		return JUMPING;
	}
};

class FallingState: public OnAirState{
public:
	char getCode(){
		return FALLING;
	}
	bool movimientoLateral;
};

class StandByState: public PersonajeState{
public:
	void handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return STANDBY;
	}
};


class WalkingState: public PersonajeState{
public:
	void handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return WALKING;
	}
};




#endif /* ESTADOS_H_ */
