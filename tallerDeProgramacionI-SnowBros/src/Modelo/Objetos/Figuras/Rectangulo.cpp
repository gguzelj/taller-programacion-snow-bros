#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){
	this->world->DestroyBody(this->body);
}

Rectangulo::Rectangulo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index)*2;
	this->ancho = parser->getAnchoObjeto(index)*2;
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;

	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / (alto*ancho);
	body->CreateFixture(&fixture);
}
