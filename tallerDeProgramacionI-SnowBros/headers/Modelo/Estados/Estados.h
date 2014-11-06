#ifndef ESTADOS_H_
#define ESTADOS_H_

#include "Box2D/Box2D.h"
#include "CharacterState.h"

#define JUMPING 'j'
#define STANDBY 's'
#define WALKING 'w'
#define FALLING 'f'
#define SHOOTING 'd'


class OnAirState: public CharacterState{
public:
	void handleInput(Character &character,SDL_Keycode input,Uint32 input_type);
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

class StandByState: public CharacterState{
public:
	void handleInput(Character &character,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return STANDBY;
	}
};

class WalkingState: public CharacterState{
public:
	void handleInput(Character &character,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return WALKING;
	}
	void caminar(Character &character);

	//bool movimientoLateralDerecha,
	//movimientoLateralIzquierda;
};

class ShootingState: public CharacterState{
public:
	void handleInput(Character &character,SDL_Keycode input,Uint32 input_type);
	char getCode(){
		return WALKING;
	}
};

#endif /* ESTADOS_H_ */
