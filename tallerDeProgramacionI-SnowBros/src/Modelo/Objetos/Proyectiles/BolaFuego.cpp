#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaFuego.h"

BolaFuego::BolaFuego(float x, float y, int potencia, b2World* world) {

	this->type = ID_BOLA_FUEGO;
	this->potencia = potencia;
	this->x = x;
	this->y = y;
	this->radio = RADIO_BOLA_FUEGO;
	this->masa = MASA_BOLA_FUEGO * 5;
	this->angulo = 0;
	this->estatico = false;
	this->world = world;
	this->destruir = false;
	this->teletransportar = false;

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	cuerpoDeCirculo.type = b2_dynamicBody;
	cuerpoDeCirculo.position.Set(x, y);
	cuerpoDeCirculo.gravityScale = 0;
	this->body = this->world->CreateBody(&cuerpoDeCirculo);

	b2FixtureDef fixture;
	fixture.density = 0.1;
	fixture.filter.groupIndex = BOLA_FUEGO_FILTER_INDEX;

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = radio;

	fixture.shape = &circleShape;
	fixture.friction = 1.0f;
	b2Fixture *fix = this->body->CreateFixture(&fixture);

	fix->SetUserData(this);

}

BolaFuego::~BolaFuego() {
}

void BolaFuego::setVelocidad(b2Vec2 velocidad) {
	this->body->SetLinearVelocity(velocidad);
}

float BolaFuego::getRadio() {
	return radio;
}

void BolaFuego::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBolaFuego(this, contact);
}

float BolaFuego::getAlto() {
	return radio * 2;
}

float BolaFuego::getAncho() {
	return radio * 2;
}

char BolaFuego::getId() {
	return BOLA_FUEGO_CODE;
}

b2Vec2 BolaFuego::getVelocidad() {
	return this->body->GetLinearVelocity();
}

b2Body* BolaFuego::getb2Body() {
	return body;
}
