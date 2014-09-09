#include "../../../../headers/Modelo/Objetos/Figuras/Paralelogramo.h"

Paralelogramo::~Paralelogramo(){
	this->world->DestroyBody(this->body);
}

Paralelogramo::Paralelogramo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;
	this->inclinacion = inclinacion;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	bool inclinacionHaciaDerecha = true;

	//definiendo ahora el fixture del paralelogramo
	b2Vec2 vertices[4];
	vertices[0].Set(0,0);
	vertices[1].Set(ancho,0);
	inclinacionHaciaDerecha ? vertices[2].Set(ancho+inclinacion,alto) : vertices[2].Set(ancho-inclinacion,alto);
	inclinacionHaciaDerecha ? vertices[3].Set(inclinacion,alto) : vertices[3].Set(-inclinacion,alto);

	b2PolygonShape shape;
	shape.Set(vertices,4);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = masa / ((ancho+inclinacion) * alto);
	body->CreateFixture(&fixture);
}
