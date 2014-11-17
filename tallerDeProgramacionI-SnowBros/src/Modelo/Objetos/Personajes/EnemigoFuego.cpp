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

	int v1 = rand() % 100;

	if (v1 <= 20)
		this->handleInput(SDLK_LEFT, SDL_KEYDOWN);
	else if (v1 <= 40)
		this->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
	else if (v1 <= 60)
		this->handleInput(SDLK_LEFT, SDL_KEYUP);
	else if (v1 <= 80)
		this->handleInput(SDLK_RIGHT, SDL_KEYUP);
	else if (v1 <= 82)
		this->handleInput(SDLK_SPACE, SDL_KEYDOWN);

}
