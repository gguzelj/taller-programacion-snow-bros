#include "../../../../headers/Modelo/Objetos/Figuras/Portal.h"

Portal::Portal(float ancho, float alto, int rot, float pos_x, float pos_y, b2World* world) {
	this->type = ID_PORTAL;
	this->ancho = ancho;
	this->alto = alto;
	this->y = pos_y;
	this->x = pos_x;
	this->angulo = rot * DEGTORAD;
	this->estatico = true;
	this->world = world;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x, y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho / 2, alto / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto * ancho);
	fixtureDef.filter.groupIndex = MURO_FILTER_INDEX;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

Portal::~Portal() {
	this->world->DestroyBody(this->body);
}

void Portal::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactPortal(this, contact);
}

float Portal::getAncho() {
	return ancho;
}

float Portal::getAlto() {
	return alto;
}

char Portal::getId() {
	return RECTANGULO_CODE;
}

void Portal::setDestination(const b2Vec2& destination) {
	this->destination = destination;
}

const b2Vec2& Portal::getDestination() {
	return destination;
}
