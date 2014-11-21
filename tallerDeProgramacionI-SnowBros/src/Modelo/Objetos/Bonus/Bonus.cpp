#include "../../../../headers/Modelo/Objetos/Bonus/Bonus.h"

Bonus::Bonus(float posicionX, float posicionY, b2World* world) {

	this->x = posicionX;
	this->y = posicionY;
	this->alto = 1.5;
	this->ancho = 1.5;
	this->masa = 1;
	this->angulo = 0;
	this->estatico = false;
	this->activado = true;
	this->world = world;
	this->teletransportar = false;

	b2BodyDef cuerpo;
	cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x, y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho / 2, alto / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto * ancho);
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);

}

void Bonus::desactivar() {
	this->activado = false;
}

bool Bonus::activo(){
	return this->activado;
}

float Bonus::getAncho(){
	return ancho;
}
float Bonus::getAlto(){
	return alto;
}

char Bonus::getId(){
	return RECTANGULO_CODE;
}
