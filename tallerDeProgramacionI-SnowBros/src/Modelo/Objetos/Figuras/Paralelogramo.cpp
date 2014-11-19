#include "../../../../headers/Modelo/Objetos/Figuras/Paralelogramo.h"

Paralelogramo::~Paralelogramo(){
	this->world->DestroyBody(this->body);
}

Paralelogramo::Paralelogramo(JsonParser *parser, int index, b2World* world){
	this->type = ID_PARALELOGRAMO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->inclinacion = 30;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;
	this->teletransportar = false;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//Definiendo ahora el fixture del paralelogramo
	float hip = alto / sin(inclinacion * DEGTORAD);

	b2Vec2 vertices[4];
	vertices[0].Set( ancho / 2 + cos(inclinacion * DEGTORAD) * hip / 2, alto / 2);  //Superior der
	vertices[1].Set( ancho / 2 - cos(inclinacion * DEGTORAD) * hip / 2, -alto / 2);	//Inferior der
	vertices[2].Set(vertices[0].x - ancho, vertices[0].y);							//Superior izq
	vertices[3].Set(vertices[1].x - ancho, vertices[1].y);							//Inferior izq

	b2PolygonShape shape;
	shape.Set(vertices,4);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = masa / (ancho * alto);
	fixtureDef.filter.groupIndex = FIGURA_FILTER_INDEX;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);

}

float Paralelogramo::getAncho(){
	return (ancho + alto / tan(M_PI / 6));
}

float Paralelogramo::getAlto(){
	return alto;
}

char Paralelogramo::getId(){
	return PARALELOGRAMO_CODE;
}
