#include "../../../../headers/Modelo/Objetos/Figuras/Paralelogramo.h"

Paralelogramo::~Paralelogramo(){
	this->world->DestroyBody(this->body);
}

Paralelogramo::Paralelogramo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index)*2;	//Para seguir con la onda del rectangulo
	this->ancho = parser->getAnchoObjeto(index)*2;
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->inclinacion = parser->getInclinacionObjeto(index);
	this->color = parser->getColorObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(0,0);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//Definiendo ahora el fixture del paralelogramo

	float hip = alto / sin(inclinacion * DEGTORAD);

	b2Vec2 vertices[4];
	vertices[0].Set( x + ancho/2 + cos(inclinacion * DEGTORAD) * hip / 2, y + alto/2);  	//Superior der
	vertices[1].Set(vertices[0].x - ancho, vertices[0].y);									//Superior izq
	vertices[2].Set( x - ancho/2 - cos(inclinacion * DEGTORAD) * hip / 2, y - alto/2);		//Inferior izq
	vertices[3].Set(vertices[2].x + ancho, vertices[2].y);									//Inferior der

	b2PolygonShape shape;
	shape.Set(vertices,4);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = masa / (ancho * alto);
	fixture.friction = 0.1f;
	body->CreateFixture(&fixture);
}
