#define VEL_POR_DEFAULT 10.0f
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

void detectarEstadoBasico(Character &character) {
	if (character.getVelocity().y <= 0.0f && character.getContactosActuales() == 0) {
		character.state = &Character::falling;
		return;
	}

	if (character.getVelocity().y > 0.0f && character.getContactosActuales() == 0) {
		character.state = &Character::jumping;
		return;
	}

	if (character.getVelocity().x != 0.0f) {
		character.state = &Character::walking;
		return;
	}

	character.state = &Character::standby;
}

void detenerMovimientos(Character &character, SDL_Keycode input) {

	char orientacion;

	switch (input) {
	case SDLK_LEFT:
		orientacion = IZQUIERDA;
		break;

	case SDLK_RIGHT:
		orientacion = DERECHA;
		break;
	}

	character.detener(orientacion);
}

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
			character.state = &Character::jumping;
			character.jump();
			break;

		case SDLK_LEFT:
			character.state = &Character::walking;
			character.caminar(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.state = &Character::walking;
			character.caminar(DERECHA);
			break;

		case SDLK_SPACE:
			character.state = &Character::shooting;
			character.disparar();
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {
		case SDLK_SPACE:
			character.movimientoDisparar = false;
			detectarEstadoBasico(character);
			break;

		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;
		}

		//Detiene si la orientacion es la misma que tenia
		if (character.detener(orientacion))
			character.state = &Character::standby;
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
			character.state = &Character::jumping;
			character.jump();
			break;

		case SDLK_LEFT:
			character.caminar(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.caminar(DERECHA);
			break;

		case SDLK_SPACE:
			character.state = &Character::shooting;
			character.disparar();
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {

		case SDLK_SPACE:
			character.movimientoDisparar = false;
			detectarEstadoBasico(character);
			break;

		case SDLK_LEFT:
			orientacion = IZQUIERDA;
			break;

		case SDLK_RIGHT:
			orientacion = DERECHA;
			break;
		}

		//Detiene si la orientacion es la misma que tenia
		if (character.detener(orientacion))
			character.state = &Character::standby;
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
			character.state = &Character::shooting;
			character.disparar();
			break;

		case SDLK_LEFT:
			character.caminar(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.caminar(DERECHA);
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
			character.movimientoDisparar = false;
			detectarEstadoBasico(character);
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

	switch (input_type) {
	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_UP:
			character.state = &Character::jumping;
			character.jump();
			break;

		case SDLK_SPACE:
			character.movimientoDisparar = true;
			break;

		case SDLK_LEFT:
			character.caminar(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.caminar(DERECHA);
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {

		case SDLK_SPACE:
			character.movimientoDisparar = false;
			detectarEstadoBasico(character);
			break;
		}

		detenerMovimientos(character, input);

		break;
	}
}

/**
 * DyingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Dying
 */
void DyingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {
	character.detener(IZQUIERDA);
	character.detener(DERECHA);
	return;
}

/**
 * PushingState: Clase encargada de manejar los eventos cuando el
 * personaje se encuentra en un estado Pushing
 */
void PushingState::handleInput(Character &character, SDL_Keycode input, Uint32 input_type) {

	switch (input_type) {

	case SDL_KEYDOWN:

		switch (input) {

		case SDLK_SPACE:
			character.kick();
			character.state = &Character::kicking;
			break;

		case SDLK_LEFT:
			character.caminar(IZQUIERDA);
			break;

		case SDLK_RIGHT:
			character.caminar(DERECHA);
			break;
		}
		break;

	case SDL_KEYUP:

		switch (input) {

		case SDLK_SPACE:
			detectarEstadoBasico(character);
		}

		detenerMovimientos(character, input);
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

		if (input == SDLK_UP) {
			character.state = &Character::jumping;
			character.jump();
			break;
		}
		break;

	case SDL_KEYUP:
		character.detener(IZQUIERDA);
		character.detener(DERECHA);

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
