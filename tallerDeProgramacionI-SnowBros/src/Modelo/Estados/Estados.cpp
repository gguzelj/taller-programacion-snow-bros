#include "../../../headers/Modelo/Estados/Estados.h"
#include "../../../headers/Modelo/Objetos/Personajes/Character.h"
#include "../../../headers/Modelo/Objetos/Personajes/Personaje.h"

StandByState Character::standby;
JumpingState Character::jumping;
WalkingState Character::walking;
FallingState Character::falling;
ShootingState Character::shooting;
DyingState Character::dying;
PushingState Character::pushing;
KickingState Character::kicking;
RollingState Character::rolling;
FlyingState Character::flying;

/**
 * StandByState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado StandBy
 */
void StandByState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	char orientacion;

	switch (input_type) {

	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_UP:
			character.jump();
			break;

		case SDLK_LEFT:
			character.move(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.move(DERECHA);
			break;

		case SDLK_SPACE:
			character.disparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dispararPortal();
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {

		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;

		case SDLK_SPACE:
			character.dejarDisparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dejarDispararPortal();
			break;

		}

		//Detiene si la orientacion es la misma que tenia
		character.detener(orientacion);
	}

}

/**
 * WalkingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado WalkingState
 */
void WalkingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	char orientacion;

	switch (input_type) {
	case SDL_KEYDOWN:
		switch (input) {

		case SDLK_UP:
			character.jump();
			break;

		case SDLK_LEFT:
			character.move(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.move(DERECHA);
			break;

		case SDLK_SPACE:
			character.disparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dispararPortal();
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {

		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;

		case SDLK_SPACE:
			character.dejarDisparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dejarDispararPortal();
			break;
		}

		character.detener(orientacion);
	}
}

/**
 * OnAirState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Falling o Jumping
 */
void OnAirState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	char orientacion;

	switch (input_type) {
	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_SPACE:
			character.disparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dispararPortal();
			break;

		case SDLK_LEFT:
			character.move(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.move(DERECHA);
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {
		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;

		case SDLK_SPACE:
			character.dejarDisparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dejarDispararPortal();
			break;

		}

		character.detener(orientacion);
		break;
	}
}

/**
 * ShootingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Shooting
 */
void ShootingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	char orientacion;

	switch (input_type) {
	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_UP:
			character.jump();
			break;

		case SDLK_LEFT:
			character.move(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.move(DERECHA);
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {
		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;

		case SDLK_SPACE:
			character.dejarDisparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dejarDispararPortal();
			break;

		}

		character.detener(orientacion);
		break;
	}
}

/**
 * DyingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Dying
 */
void DyingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {
	return;
}

/**
 * PushingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Pushing
 */
void PushingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	char orientacion;

	switch (input_type) {

	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_SPACE:
			character.kick();
			character.state = &Character::kicking;
			break;

		case SDLK_LEFT:
			character.move(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.move(DERECHA);
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {
		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;

		case SDLK_SPACE:
			character.dejarDisparar();
			break;

		case SDLK_RCTRL:
			if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
				personaje->dejarDispararPortal();
			break;

		}

		character.detener(orientacion);
		break;
	}
}

/**
 * KickingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Kicking
 */
void KickingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {

	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_UP:
			character.state = &Character::jumping;
			character.jump();
			break;
		}
		break;

	case SDL_KEYUP:
		character.detener(IZQUIERDA);
		character.detener(DERECHA);
		character.dejarDisparar();
		if (Personaje* personaje = dynamic_cast<Personaje*>(&character))
			personaje->dejarDispararPortal();
	}
}

/**
 * RollingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado RollingState
 */
void RollingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {
	if (Personaje* personaje = dynamic_cast<Personaje*>(&character)) {
		switch (input_type) {
		case SDL_KEYDOWN:
			if (input == SDLK_UP) {
				personaje->setArrastrado(false);
				break;
			}
		}
	}
}
