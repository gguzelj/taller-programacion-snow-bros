#include "../../../../headers/Modelo/Objetos/Personajes/EnemigoFuego.h"
#include "../../../../headers/Modelo/Escenario.h"

EnemigoFuego::EnemigoFuego(JsonParser *parser, int index, Escenario* escenario) :
		Enemigo(parser, index, escenario) {

	this->type = ID_ENEMIGO_FUEGO;

}

EnemigoFuego::~EnemigoFuego() {
}

void EnemigoFuego::shoot() {

	if (!movimientoDisparar || shootCooldown > 0)
		return;

	BolaFuego *bola;
	float x = getX();
	float y = getY();

	shootCooldown = 20;

	x += (orientacion == IZQUIERDA) ? -1 : 1;

	bola = new BolaFuego(x, y, 1, this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	vel.x -= (orientacion == IZQUIERDA) ? aceleracion * 4 : -aceleracion * 4;
	vel.y = 0;
	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);
}

void EnemigoFuego::mover() {

	if (movimientoDisparar)
		movimientoDisparar = false;

	int v1 = rand() % 100;
		if (v1 <= 20)
			this->handleInput(SDLK_LEFT, SDL_KEYDOWN);
		else if (v1 <= 40)
			this->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
		else if (v1 <= 60)
			this->handleInput(SDLK_LEFT, SDL_KEYUP);
		else if (v1 <= 80)
			this->handleInput(SDLK_RIGHT, SDL_KEYUP);
		else if (v1 >= 82)
			this->handleInput(SDLK_SPACE, SDL_KEYDOWN);
}
