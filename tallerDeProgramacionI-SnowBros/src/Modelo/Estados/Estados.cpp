#define VEL_POR_DEFAULT 10.0f
#include "../../../headers/Modelo/Estados/Estados.h"
#include "../../../headers/Modelo/Objetos/Personajes/Character.h"

StandByState Character::standby;
JumpingState Character::jumping;
WalkingState Character::walking;
FallingState Character::falling;
ShootingState Character::shooting;
DyingState Character::dying;
PushingState Character::pushing;

void cambiarOrientacionAlDejarDePresionarUnaTecla(Character &character) {
	switch (character.getOrientacion()) {
	case IZQUIERDA:
		if (character.movimientoLateralIzquierda == false)
			character.setOrientacion(DERECHA);
		break;
	case DERECHA:
		if (character.movimientoLateralDerecha == false)
			character.setOrientacion(IZQUIERDA);
		break;
	}
}

//devuelve true si se detuvo o false si no hizo nada
bool detenerMovimientoHorizontal(Character* character, SDL_Keycode input) {
	switch (character->getOrientacion()) {
	case IZQUIERDA:
		switch (input) {
		case SDLK_LEFT:
			character->stop();
			character->movimientoLateralIzquierda = false;
			return true;
		case SDLK_RIGHT:
			character->movimientoLateralDerecha = false;
			break;
		}
		break;

	case DERECHA:
		switch (input) {
		case SDLK_RIGHT:
			character->stop();
			character->movimientoLateralDerecha = false;
			return true;
		case SDLK_LEFT:
			character->movimientoLateralIzquierda = false;
			break;
		}
		break;
	}
	return false;
}

void OnAirState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {
	switch (input_type) {
	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_SPACE: {
			character.state = &Character::shooting;
			character.disparar();
			break;
		}

		case SDLK_LEFT:
			if (!character.movimientoLateralDerecha) {
				character.moveLeft();
				character.setOrientacion(IZQUIERDA);
			}
			character.movimientoLateralIzquierda = true;
			break;

		case SDLK_RIGHT:
			if (!character.movimientoLateralIzquierda) {
				character.moveRight();
				character.setOrientacion(DERECHA);
			}
			character.movimientoLateralDerecha = true;
			break;
		}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			character.debeSaltar = false;
		detenerMovimientoHorizontal(&character, input);
		if (character.movimientoLateralIzquierda || character.movimientoLateralDerecha)
			cambiarOrientacionAlDejarDePresionarUnaTecla(character);
		break;
	}
}

void saltar(Character& character) {
	if (character.getJumpCooldown() <= 0) {
		character.debeSaltar = true;
		character.state = &Character::jumping;
		character.jump();
	}
}

void StandByState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {
	case SDL_KEYDOWN: {
		switch (input) {

		case SDLK_UP: {
			saltar(character);
			break;
		}

		case SDLK_LEFT: {
			character.movimientoLateralIzquierda = true;
			character.state = &Character::walking;
			character.moveLeft();
			character.setOrientacion(IZQUIERDA);
			break;
		}

		case SDLK_RIGHT: {
			character.movimientoLateralDerecha = true;
			character.state = &Character::walking;
			character.moveRight();
			character.setOrientacion(DERECHA);
			break;
		}

		case SDLK_SPACE: {
			character.state = &Character::shooting;
			character.disparar();
			break;
		}
		}
	}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			character.debeSaltar = false;

	}
}

void WalkingState::caminar(Character &character) {
	switch (character.getOrientacion()) {
	case IZQUIERDA: {
		character.moveLeft();
		break;
	}
	case DERECHA: {
		character.moveRight();
		break;
	}
	}
}

void WalkingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {
	case SDL_KEYDOWN:
		switch (input) {
		case SDLK_UP:
			saltar(character);
			break;
		case SDLK_LEFT:
			character.movimientoLateralIzquierda = true;
			break;
		case SDLK_RIGHT:
			character.movimientoLateralDerecha = true;
			break;
		case SDLK_SPACE: {
			character.state = &Character::shooting;
			character.disparar();
			break;
		}
		}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			character.debeSaltar = false;
		detenerMovimientoHorizontal(&character, input);
		if (character.movimientoLateralDerecha == false && character.movimientoLateralIzquierda == false)
			character.state = &Character::standby;
		else
			cambiarOrientacionAlDejarDePresionarUnaTecla(character);
		break;
	}
}

void ShootingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {
	case SDL_KEYDOWN:

			switch (input) {

			case SDLK_SPACE: {
				//character.state = &Character::shooting;
				character.disparar();
				break;
			}

			case SDLK_LEFT:
				if (!character.movimientoLateralDerecha) {
					character.moveLeft();
					character.setOrientacion(IZQUIERDA);
				}
				character.movimientoLateralIzquierda = true;
				break;

			case SDLK_RIGHT:
				if (!character.movimientoLateralIzquierda) {
					character.moveRight();
					character.setOrientacion(DERECHA);
				}
				character.movimientoLateralDerecha = true;
				break;
			}
			break;

	case SDL_KEYUP:
		detenerMovimientoHorizontal(&character, input);
		if(input == SDLK_SPACE){
			if (character.movimientoLateralDerecha == false && character.movimientoLateralIzquierda == false){
				if (character.getVelocity().y <= 0.0f && character.getCantidadDeContactosActuales() == 0)
					character.state = &Character::falling;
				else
					character.state = &Character::standby;
			}
			else
				character.state = &Character::walking;
		}
	}

}
void PushingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {
	switch (input_type) {
	case SDL_KEYDOWN:
			switch (input) {
			case SDLK_SPACE: {
				character.empujar();
				break;
			}

			case SDLK_LEFT:
				if (!character.movimientoLateralDerecha) {
					character.pushLeft();
					character.setOrientacion(IZQUIERDA);
				}
				character.movimientoLateralIzquierda = true;
				break;

			case SDLK_RIGHT:
				if (!character.movimientoLateralIzquierda) {
					character.pushRight();
					character.setOrientacion(DERECHA);
				}
				character.movimientoLateralDerecha = true;
				break;
			}
			break;

	case SDL_KEYUP:
		if(input == SDLK_SPACE){
			if (character.movimientoLateralDerecha == false && character.movimientoLateralIzquierda == false){
				if (character.getVelocity().y <= 0.0f && character.getCantidadDeContactosActuales() == 0){
					character.state = &Character::falling;
					character.noAtravezarPlataformas();
				}
				else
					character.state = &Character::standby;
			}
			else
				character.state = &Character::walking;
		}
		else if((input == SDLK_LEFT)||(input == SDLK_RIGHT)){
			character.state = &Character::walking;
		}
	}

}
