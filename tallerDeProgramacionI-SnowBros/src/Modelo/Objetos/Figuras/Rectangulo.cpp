#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){
	this->world->DestroyBody(this->body);
}

Rectangulo::Rectangulo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	std::cout << "Dentro de Rectangulo, estatico? " << estatico << std::endl;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;

	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho,alto);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = masa / (ancho * alto);
	body->CreateFixture(&fixture);
}
