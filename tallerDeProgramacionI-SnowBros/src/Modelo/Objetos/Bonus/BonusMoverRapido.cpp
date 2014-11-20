#include "../../../../headers/Modelo/Objetos/Bonus/BonusMoverRapido.h"

BonusMoverRapido::BonusMoverRapido(float posicionX, float posicionY, b2World* world){
	this->type = ID_RECTANGULO;
	this->x = posicionX;
	this->y = posicionY;
	this->alto = 3.14;
	this->ancho = 3.14;
	this->masa = 1;
	this->angulo = 0;
	this->estatico = false;
	this->world = world;
	this->teletransportar = false;

	b2BodyDef cuerpo;
	cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto*ancho);
//	fixtureDef.filter.groupIndex = FIGURA_FILTER_INDEX;	//ver si se remueve
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

BonusMoverRapido::~BonusMoverRapido(){

}

void BonusMoverRapido::desactivar(){
	this->activado = false;
}

bool BonusMoverRapido::activo(){
	return this->activado;
}

float BonusMoverRapido::getAncho(){
	return ancho;
}
float BonusMoverRapido::getAlto(){
	return alto;
}
char BonusMoverRapido::getId(){
	return 'c';
}
