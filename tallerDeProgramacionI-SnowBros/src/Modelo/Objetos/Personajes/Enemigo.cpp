#include "../../../../headers/Modelo/Objetos/Personajes/Enemigo.h"
#include "../../../../headers/Modelo/Escenario.h"
#include <ctime>
#define ORIENTACION_INICIAL 'l'

Enemigo::Enemigo(JsonParser *parser, int index, Escenario* escenario) {
	//Parametros generales
	this->escenario_ = escenario;
	this->jumpCooldown = 0;
	this->world = escenario->getWorld();
	this->aceleracion = 7.0f;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->state = &Enemigo::standby;
	this->state = nullptr;
	this->orientacion = ORIENTACION_INICIAL;
	this->movimientoDerecha = false;
	this->movimientoIzquierda = false;
	this->tiempoDeImpactoDeLaUltimaBola = 0.0f;
	this->esAtravezable = false;
	this->type = ID_ENEMIGO;
	this->lives = 5;
	this->ancho = MITAD_ANCHO_ENEMIGO;
	this->alto = MITAD_ALTO_ENEMIGO;
	this->nivelDeCongelamiento = 0;
	this->puedeEmpujar = false;
	this->estaVivo = true;
	this->puntos = 50;

	//Parametros para controlar los contactos
	this->contactosActuales = 0;
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

	//Defino body del Enemigo
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angle = 0;
	bodyDef.position.Set(x, y);
	bodyDef.gravityScale = 4;
	this->body = this->world->CreateBody(&bodyDef);

	//Defino el Shape del enemigo
	b2PolygonShape shapeDelEnemigo;
	shapeDelEnemigo.SetAsBox(ancho, alto);

	//Definimos el fixture del Enemigo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDelEnemigo;
	fixtureDef.density = 0.5;
	fixtureDef.filter.groupIndex = ENEMIGO_FILTER_INDEX;
	fixtureDef.friction = 0.96;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(-ancho + 0.01, 0), 0);
	fixtureDef.friction = 0.01f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(ancho - 0.01, 0), 0);
	fixtureDef.friction = 0.01f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelEnemigo.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto),
			0);
	;
	fixtureDef.friction = 0.0019f;
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

}

Enemigo::~Enemigo() {
	this->world->DestroyBody(this->body);
}

void Enemigo::disparar() {

}

void Enemigo::empujar() {

}

void Enemigo::morir() {

	this->estaVivo = false;

	//Ahora creamos la nueva bola
	b2Vec2 vel;
	BolaEnemigo *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaEnemigo(getX() - 1, getY() + MITAD_ALTO_ENEMIGO,
				this->world);
	else
		bola = new BolaEnemigo(getX() + 1, getY() + MITAD_ALTO_ENEMIGO,
				this->world);

	vel.x = (orientacion == IZQUIERDA) ? -15 : 15;
	vel.y = 5;
	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

	//Lanzamos un thread para que muera la bola
	std::thread t(&BolaEnemigo::morir, bola);
	t.detach();
}

void Enemigo::morirDelay() {
	sleep(1);
	this->estaVivo = false;
}

void Enemigo::beginContactBolaEnemigo(BolaEnemigo* bola, b2Contact* contact) {

	//Lanzamos un thread para que muera el enemigo
	std::thread r(&Enemigo::morirDelay, this);
	r.detach();

	//Lanzamos el enemigo por los aires
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.y = 70;
	velocidadActual.x *= -1;
	this->body->SetLinearVelocity(velocidadActual);

}

void Enemigo::handleInput(SDL_Keycode input, Uint32 input_type) {
	if (nivelDeCongelamiento > 0)
		return;
	state->handleInput(*this, input, input_type);
}

void Enemigo::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactEnemigo(this, contact);
}

void Enemigo::beginContactBolaNieve(BolaNieve* bola, b2Contact* contact) {

	time(&tiempoDeImpactoDeLaUltimaBola);

	//Si estaba semicongelado, lo congelamos un poco mas
	if (this->nivelDeCongelamiento > 0) {
		this->nivelDeCongelamiento += bola->potencia;
		if (this->nivelDeCongelamiento > NIVEL_CONGELAMIENTO_MAX)
			this->nivelDeCongelamiento = NIVEL_CONGELAMIENTO_MAX;
	}

	//Si no estaba congelado, empezamos a congelar
	if (this->nivelDeCongelamiento == 0) {
		this->state = &Enemigo::standby;
		this->nivelDeCongelamiento += bola->potencia;
		std::thread t(&Enemigo::congelar, this);
		t.detach();
	}
}

void Enemigo::congelar() {
	float tiempoDeEsperaMaximo = 5.0f;
	aceleracion = 0;
	while (nivelDeCongelamiento != 0) {
		//En caso de que este hecho bola de nieve, lo hacemos
		//No atravezable, para que pueda empujarlo
		esAtravezable = (nivelDeCongelamiento != NIVEL_CONGELAMIENTO_MAX);

		if (difftime(time(nullptr), tiempoDeImpactoDeLaUltimaBola)
				> tiempoDeEsperaMaximo) {
			this->nivelDeCongelamiento -= 2;
			if (this->nivelDeCongelamiento < 0)
				this->nivelDeCongelamiento = 0;
			time(&tiempoDeImpactoDeLaUltimaBola);
		}
		//En caso de que este hecho bola de nieve, lo hacemos
		//No atravezable, para que pueda empujarlo
		esAtravezable = (nivelDeCongelamiento != NIVEL_CONGELAMIENTO_MAX);
	}
	esAtravezable = false;
	aceleracion = 7.0f;
}

bool Enemigo::estaCongelado() {
	return (nivelDeCongelamiento == NIVEL_CONGELAMIENTO_MAX);
}

void Enemigo::jump() {
	if (this->nivelDeCongelamiento == 0) {
		if (this->jumpCooldown <= 0) {
			this->jumpCooldown = 18;
			b2Vec2 velocidadActual = this->body->GetLinearVelocity();
			velocidadActual.y = 25;
			this->body->SetLinearVelocity(velocidadActual);
		}
	}
}

void Enemigo::hacerAtravezable() {
	this->cambiarFilterIndex(PERSONAJE_FILTER_INDEX);
}

void Enemigo::hacerNoAtravezable() {
	this->cambiarFilterIndex(ENEMIGO_FILTER_INDEX);
}

int Enemigo::getNivelDeCongelamiento() {
	return nivelDeCongelamiento;
}

b2Body* Enemigo::getb2Body() {
	return this->body;
}

int Enemigo::getPuntos() {
	return puntos;
}

void Enemigo::controlarEstado() {
	Character::controlarEstado();

	//Analizamos si el enemigo es atravezable
	if (esAtravezable)
		hacerAtravezable();
	else
		hacerNoAtravezable();
}

void Enemigo::mover() {
	int i = 0;
	std::list<Personaje*>* personajes_ = escenario_->getPersonajes();

	float posicionesX[4];
	float posicionesY[4];
	for (auto personaje = personajes_->begin(); personaje != personajes_->end();
			++personaje) {
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
		if ((posicionPersonajeY + 1) < posicionEnemigoY
				&& this->getNivelDeCongelamiento() == 0) {
			this->handleInput(SDLK_UP, SDL_KEYUP);
			this->atravezarPlataformas();
		}
	}

}
void Escenario::movimientoDelEnemigo(Enemigo* enemigo) {
}
