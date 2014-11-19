#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaPortal.h"

BolaPortal::BolaPortal(float x, float y, int potencia, b2World* world) {

	this->type = ID_BOLA_PORTAL;

	this->potencia = potencia;
	this->x = x;
	this->y = y;
	this->radio = RADIO_BOLA_PORTAL;
	this->masa = MASA_BOLA_PORTAL * 5;
	this->angulo = 0;
	this->estatico = false;
	this->teletransportar = false;
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

BolaPortal::~BolaPortal() {
	// TODO Auto-generated destructor stub
}


void BolaPortal::setVelocidad(b2Vec2 velocidad) {
	this->body->SetLinearVelocity(velocidad);
}

float BolaPortal::getRadio() {
	return radio;
}

float BolaPortal::getAlto() {
	return radio * 2;
}

float BolaPortal::getAncho() {
	return radio * 2;
}

char BolaPortal::getId() {
	return BOLA_PORTAL_CODE;
}
