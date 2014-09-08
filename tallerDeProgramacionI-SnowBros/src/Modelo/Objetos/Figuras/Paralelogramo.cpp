#include "../../../../headers/Modelo/Objetos/Figuras/Paralelogramo.h"

Paralelogramo::~Paralelogramo(){
	this->world->DestroyBody(this->body);
}

Paralelogramo::Paralelogramo(int x, int y, int ancho, int alto, int inclinacion, bool inclinacionHaciaDerecha, std::string color, int angulo, float masa, bool esDinamico, b2World* world){
	this->x = x;
	this->y = y;
	this->alto = alto;
	this->ancho = ancho;
	this->inclinacion = inclinacion;
	this->masa = masa;
	this->angulo = angulo;
	this->estatico = estatico;
	this->color = color;
	this->world = world;

	b2BodyDef cuerpo;
	esDinamico ? cuerpo.type = b2_dynamicBody : cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

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
