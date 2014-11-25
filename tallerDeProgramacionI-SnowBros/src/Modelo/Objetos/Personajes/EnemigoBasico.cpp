#include "../../../../headers/Modelo/Objetos/Personajes/EnemigoBasico.h"
#include "../../../../headers/Modelo/Escenario.h"

EnemigoBasico::EnemigoBasico(JsonParser *parser, int index, Escenario* escenario) :
		Enemigo(parser, index, escenario) {

	this->type = ID_ENEMIGO_BASICO;

}

EnemigoBasico::~EnemigoBasico() {

}

void EnemigoBasico::mover() {
	int i = 0;
	std::list<Personaje*>* personajes_ = escenario_->getPersonajes();

	float posicionesX[4];
	float posicionesY[4];
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		posicionesX[i] = (*personaje)->getX();
		posicionesY[i] = (*personaje)->getY();
		i++;
	}
	float posicionPersonajeX = posicionesX[0];
	float posicionPersonajeY = posicionesY[0];
	float posicionEnemigoX = this->getX();
	float posicionEnemigoY = this->getY();

	int v1 = rand() % 100;
	if (v1 < 45) {
		v1 = rand() % 100;
		if (v1 <= 25)
			this->handleInput(SDLK_LEFT, SDL_KEYDOWN);
		else {
			if (v1 <= 50)
				this->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
			else {
				if (v1 <= 75)
					this->handleInput(SDLK_RIGHT, SDL_KEYUP);
				else
					this->handleInput(SDLK_LEFT, SDL_KEYUP);
			}
		}
	} else if (v1 < 50) {
		if (posicionPersonajeX < posicionEnemigoX) {
			this->handleInput(SDLK_RIGHT, SDL_KEYUP);
			this->handleInput(SDLK_LEFT, SDL_KEYDOWN);
		}
		if (posicionPersonajeX > posicionEnemigoX) {
			this->handleInput(SDLK_LEFT, SDL_KEYUP);
			this->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
		}
		if ((posicionPersonajeY - 3) > posicionEnemigoY) {
			this->handleInput(SDLK_UP, SDL_KEYDOWN);
		}
	}
	if (v1 > 5) {
		if ((posicionPersonajeY + 1) < posicionEnemigoY && this->getNivelDeCongelamiento() == 0) {
			this->handleInput(SDLK_UP, SDL_KEYUP);
		}
	}

}
