#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaEnemigo.h"

BolaEnemigo::BolaEnemigo(float x, float y, b2World* world) {
	this->type = ID_BOLA_NIEVE_ENEMIGO;

	this->x = x;
	this->y = y;
	this->radio = RADIO_BOLA_NIEVE_ENEMIGO;
	this->masa = MASA_BOLA_NIEVE_ENEMIGO;
	this->angulo = 0;
	this->estatico = false;
	this->destruir = false;
	this->world = world;

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	cuerpoDeCirculo.type = b2_dynamicBody;
	cuerpoDeCirculo.position.Set(x, y);
	cuerpoDeCirculo.gravityScale = 8;
	this->body = this->world->CreateBody(&cuerpoDeCirculo);

	b2FixtureDef fixture;
	fixture.filter.groupIndex = BOLA_ENEMIGO_FILTER_INDEX;
	fixture.density = 0.1;
	fixture.restitution = 0.5;

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = radio;

	fixture.shape = &circleShape;
	fixture.friction = 1.0f;
	b2Fixture *fix = this->body->CreateFixture(&fixture);

	fix->SetUserData(this);
}

BolaEnemigo::~BolaEnemigo() {
}

void BolaEnemigo::setVelocidad(b2Vec2 velocidad) {
	this->body->SetLinearVelocity(velocidad);
}

float BolaEnemigo::getRadio() {
	return radio;
}

float BolaEnemigo::getAlto() {
	return radio * 2;
}

float BolaEnemigo::getAncho() {
	return radio * 2;
}

char BolaEnemigo::getId() {
	return BOLA_NIEVE_CODE;
}

void BolaEnemigo::morir(){
	sleep(15);
	destruir = true;
}

void BolaEnemigo::beginContact(Figura *fig, b2Contact* contact){
	fig->beginContactBolaEnemigo(this, contact);
}

void BolaEnemigo::cambiarFilterIndex(int16 groupIndex){
	b2Filter filter;
	filter.groupIndex = groupIndex;

	//Recorremos todos los fixtures del objeto y cambiamos el filter
	for (b2Fixture* fix = this->body->GetFixtureList(); fix; fix = fix->GetNext()) {
		fix->SetFilterData(filter);
	}

}
