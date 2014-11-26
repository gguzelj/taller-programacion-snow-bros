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
	this->orientacion = ORIENTACION_INICIAL;
	this->movimientoDisparar = false;
	this->movimientoDerecha = false;
	this->movimientoIzquierda = false;
	this->teletransportar = false;
	this->tiempoDeImpactoDeLaUltimaBola = 0.0f;
	this->esAtravezable = false;
	this->lives = 5;
	this->ancho = MITAD_ANCHO_ENEMIGO;
	this->alto = MITAD_ALTO_ENEMIGO;
	this->nivelDeCongelamiento = 0;
	this->estaVivo = true;
	this->puntos = 50;

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
	fixtureDef.density = 5;
	fixtureDef.restitution = 0;
	fixtureDef.filter.groupIndex = ENEMIGO_FILTER_INDEX;
	fixtureDef.friction = 0.96;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(-ancho + 0.01, 0), 0);
	fixtureDef.friction = 0.001f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(ancho - 0.01, 0), 0);
	fixtureDef.friction = 0.001f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelEnemigo.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);
	fixtureDef.friction = 0.9f;
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

}

Enemigo::~Enemigo() {
	//this->world->DestroyBody(this->body);
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
		bola = new BolaEnemigo(getX() - 2, getY(), world, escenario_);
	else
		bola = new BolaEnemigo(getX() + 2, getY(), world, escenario_);

	vel.x = (orientacion == IZQUIERDA) ? -35 : 35;
	vel.y = 0;
	bola->setVelocidad(vel);

	escenario_->agregarProyectil(bola);

	//Lanzamos un thread para que muera la bola
	std::thread t(&BolaEnemigo::morir, bola);
	t.detach();
}

void Enemigo::morirDelay() {
	sleep(0.3f);
	this->estaVivo = false;
//	Que solo lo cree la bola no si choca con un enemigo
//	if (escenario_->debeCrearBonus() )
//		escenario_->agregarBonus(escenario_->crearBonus(this));
}

void Enemigo::beginContactBolaEnemigo(BolaEnemigo* bola, b2Contact* contact) {
	//Lanzamos el enemigo por los aires
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.y = 70;
	velocidadActual.x *= -1;
	this->body->SetLinearVelocity(velocidadActual);
	//Lanzamos un thread para que muera el enemigo
	std::thread r(&Enemigo::morirDelay, this);
	r.detach();
}

void Enemigo::handleInput(SDL_Keycode input, Uint32 input_type) {
	if (nivelDeCongelamiento > 0)
		return;
	state->handleInput(*this, input, input_type);
}

void Enemigo::endContact(Figura* figura, b2Contact* contact) {
	Character::endContact(figura, contact);
	figura->endContactEnemigo(this, contact);
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

		if (difftime(time(nullptr), tiempoDeImpactoDeLaUltimaBola) > tiempoDeEsperaMaximo) {
			this->nivelDeCongelamiento -= 2;
			if (this->nivelDeCongelamiento < 0)
				this->nivelDeCongelamiento = 0;
			time(&tiempoDeImpactoDeLaUltimaBola);
		}
	}
	aceleracion = 7.0f;
}

bool Enemigo::estaCongelado() {
	return (nivelDeCongelamiento == NIVEL_CONGELAMIENTO_MAX);
}

void Enemigo::jump() {
	atravezarPlataformas();
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

void Enemigo::noAtravezarPlataformas() {
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

	if (nivelDeCongelamiento > 0 && !estaCongelado())
		hacerAtravezable();
	else if (state == &Character::falling)
		this->cambiarFilterIndex(ENEMIGO_FILTER_INDEX);

	if (estaCongelado()) {
		noAtravezarPlataformas();
	}
}

void Enemigo::mover() {
}
