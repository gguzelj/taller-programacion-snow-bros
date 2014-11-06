#define VEL_POR_DEFAULT 10.0f
#include "../../../headers/Modelo/Estados/Estados.h"
#include "../../../headers/Modelo/Objetos/Personaje.h"

StandByState Personaje::standby;
JumpingState Personaje::jumping;
WalkingState Personaje::walking;
FallingState Personaje::falling;
ShootingState Personaje::shooting;

void cambiarOrientacionAlDejarDePresionarUnaTecla(Personaje &personaje) {
	switch (personaje.getOrientacion()) {
	case IZQUIERDA:
		if (personaje.movimientoLateralIzquierda == false)
			personaje.setOrientacion(DERECHA);
		break;
	case DERECHA:
		if (personaje.movimientoLateralDerecha == false)
			personaje.setOrientacion(IZQUIERDA);
		break;
	}
}

//devuelve true si se detuvo o false si no hizo nada
bool detenerMovimientoHorizontal(Personaje* personaje, SDL_Keycode input) {
	switch (personaje->getOrientacion()) {
	case IZQUIERDA:
		switch (input) {
		case SDLK_LEFT:
			personaje->stop();
			personaje->movimientoLateralIzquierda = false;
			return true;
		case SDLK_RIGHT:
			personaje->movimientoLateralDerecha = false;
			break;
		}
		break;

	case DERECHA:
		switch (input) {
		case SDLK_RIGHT:
			personaje->stop();
			personaje->movimientoLateralDerecha = false;
			return true;
		case SDLK_LEFT:
			personaje->movimientoLateralIzquierda = false;
			break;
		}
		break;
	}
	return false;
}

void OnAirState::handleInput(Personaje &personaje, SDL_Keycode input, Uint32 input_type) {
	switch (input_type) {
	case SDL_KEYDOWN:

		switch (input) {
		case SDLK_LEFT:
			if (!personaje.movimientoLateralDerecha) {
				personaje.moveLeft();
				personaje.setOrientacion(IZQUIERDA);
			}
			personaje.movimientoLateralIzquierda = true;
			break;

		case SDLK_RIGHT:
			if (!personaje.movimientoLateralIzquierda) {
				personaje.moveRight();
				personaje.setOrientacion(DERECHA);
			}
			personaje.movimientoLateralDerecha = true;
			break;
		}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			personaje.debeSaltar = false;
		detenerMovimientoHorizontal(&personaje, input);
		if (personaje.movimientoLateralIzquierda || personaje.movimientoLateralDerecha)
			cambiarOrientacionAlDejarDePresionarUnaTecla(personaje);
		break;

	}

}

void saltar(Personaje& personaje) {
	if (personaje.getJumpCooldown() <= 0) {
		personaje.debeSaltar = true;
		personaje.state = &Personaje::jumping;
		personaje.jump();
	}
}

void StandByState::handleInput(Personaje &personaje, SDL_Keycode input, Uint32 input_type) {
	switch (input_type) {
	case SDL_KEYDOWN: {
		switch (input) {

		case SDLK_UP: {
			saltar(personaje);
			break;
		}

		case SDLK_LEFT: {
			personaje.movimientoLateralIzquierda = true;
			personaje.state = &Personaje::walking;
			personaje.moveLeft();
			personaje.setOrientacion(IZQUIERDA);
			break;
		}

		case SDLK_RIGHT: {
			personaje.movimientoLateralDerecha = true;
			personaje.state = &Personaje::walking;
			personaje.moveRight();
			personaje.setOrientacion(DERECHA);
			break;
		}

		case SDLK_SPACE: {
			personaje.state = &Personaje::shooting;
			personaje.tirarBolaNieve();
			break;
		}
		}
	}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			personaje.debeSaltar = false;

	}
}

void WalkingState::caminar(Personaje &personaje) {
	switch (personaje.getOrientacion()) {
	case IZQUIERDA: {
		personaje.moveLeft();
		break;
	}
	case DERECHA: {
		personaje.moveRight();
		break;
	}
	}
}

void WalkingState::handleInput(Personaje &personaje, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {
	case SDL_KEYDOWN:
		switch (input) {
		case SDLK_UP:
			saltar(personaje);
			break;
		case SDLK_LEFT:
			personaje.movimientoLateralIzquierda = true;
			break;
		case SDLK_RIGHT:
			personaje.movimientoLateralDerecha = true;
			break;

		}
		break;

	case SDL_KEYUP:
		if (input == SDLK_UP)
			personaje.debeSaltar = false;
		detenerMovimientoHorizontal(&personaje, input);
		if (personaje.movimientoLateralDerecha == false && personaje.movimientoLateralIzquierda == false)
			personaje.state = &Personaje::standby;
		else
			cambiarOrientacionAlDejarDePresionarUnaTecla(personaje);
		break;
	}
}

void ShootingState::handleInput(Personaje &personaje, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {
	case SDL_KEYUP:
		personaje.state = &Personaje::standby;
	}

}
