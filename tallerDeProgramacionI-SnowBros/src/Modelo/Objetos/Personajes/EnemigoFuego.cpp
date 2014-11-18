#include "../../../../headers/Modelo/Objetos/Personajes/EnemigoFuego.h"
#include "../../../../headers/Modelo/Escenario.h"

EnemigoFuego::EnemigoFuego(JsonParser *parser, int index, Escenario* escenario) :
		Enemigo(parser, index, escenario) {

	this->type = ID_ENEMIGO_FUEGO;

}

EnemigoFuego::~EnemigoFuego() {
}

void EnemigoFuego::shoot() {

	BolaFuego *bola;
	float x = getX();
	float y = getY() + MITAD_ALTO_ENEMIGO;

	if (!movimientoDisparar || shootCooldown > 0)
		return;

	shootCooldown = 10;

	x += (orientacion == IZQUIERDA) ? -1 : 1;

	bola = new BolaFuego(x, y, 1, this->world);

	this->escenario_->agregarProyectil(bola);

	//Lanzamos un thread para que muera la bola
	std::thread t(&BolaFuego::morir, bola);
	t.detach();

}

void EnemigoFuego::mover() {

	if (movimientoDisparar)
		movimientoDisparar = false;

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
	}
}
