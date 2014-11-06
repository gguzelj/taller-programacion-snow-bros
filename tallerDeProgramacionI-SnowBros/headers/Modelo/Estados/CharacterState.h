/*
 * PersonajeState.h
 *
 *  Created on: 18/9/2014
 *      Author: anonimo
 */

#ifndef PERSONAJESTATE_H_
#define PERSONAJESTATE_H_
#include "SDL2/SDL.h"

class Character;


class CharacterState {
public:
	CharacterState();
	virtual ~CharacterState();
	virtual void handleInput(Character &character,SDL_Keycode input,Uint32 input_type) = 0;
	//cada estado tiene un codigo.
	virtual char getCode() = 0;
};

#endif /* PERSONAJESTATE_H_ */
