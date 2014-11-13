#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaNieve.h"

BolaNieve::BolaNieve(float x, float y, int potencia, b2World* world) {

	this->type = ID_BOLA_NIEVE;

	this->potencia = potencia;
	this->x = x;
	this->y = y;
	this->radio = RADIO_BOLA_NIEVE;
	this->masa = MASA_BOLA_NIEVE * 5;
	this->angulo = 0;
	this->estatico = false;
	this->world = world;

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	cuerpoDeCirculo.type = b2_dynamicBody;
	cuerpoDeCirculo.position.Set(x, y);
	cuerpoDeCirculo.gravityScale = 8;
	this->body = this->world->CreateBody(&cuerpoDeCirculo);

	b2FixtureDef fixture;
	fixture.density = 0.1;

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = radio;

	fixture.shape = &circleShape;
	fixture.friction = 1.0f;
	b2Fixture *fix = this->body->CreateFixture(&fixture);

	fix->SetUserData(this);

}

BolaNieve::~BolaNieve() {

}

void BolaNieve::setVelocidad(b2Vec2 velocidad) {
	this->body->SetLinearVelocity(velocidad);
}

float BolaNieve::getRadio() {
	return radio;
}

void BolaNieve::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBolaNieve(this, contact);
}

float BolaNieve::getAlto() {
	return radio * 2;
}

float BolaNieve::getAncho() {
	return radio * 2;
}

char BolaNieve::getId() {
	return BOLA_NIEVE_CODE;
}
