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
#define SHOOTING 'd'


class OnAirState: public PersonajeState{
public:
	void handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type);
};

class JumpingState: public OnAirState{
public:
	char getCode(){
		return JUMPING;
	}
	//bool debeSaltar;
};

class FallingState: public OnAirState{
public:
	char getCode(){
		return FALLING;
	}
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
	void caminar(Personaje &personaje);

	//bool movimientoLateralDerecha,
	//movimientoLateralIzquierda;
};

class ShootingState: public PersonajeState{
public:
	void handleInput(Personaje &personaje,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return WALKING;
	}
};

#endif /* ESTADOS_H_ */
