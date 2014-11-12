#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){
	this->world->DestroyBody(this->body);
}
/*
 * Este constructor se va a usar para todas las figuras rectangulo que vengan del json.
 */
Rectangulo::Rectangulo(JsonParser *parser, int index, b2World* world){
	this->type = ID_RECTANGULO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto*ancho);
	fixtureDef.filter.groupIndex = FIGURA_FILTER_INDEX;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

Rectangulo::Rectangulo(float ancho, float alto, int rot, float pos_x, float pos_y, b2World* world){
	this->type = ID_RECTANGULO;
	this->ancho = ancho;
	this->alto = alto;
	this->y = pos_y;
	this->x = pos_x;
	this->angulo = rot * DEGTORAD;
	this->estatico = true;
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto*ancho);
	fixtureDef.filter.groupIndex = MURO_FILTER_INDEX;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

float Rectangulo::getAncho(){
	return ancho;
}

float Rectangulo::getAlto(){
	return alto;
}

char Rectangulo::getId(){
	return RECTANGULO_CODE;
}
