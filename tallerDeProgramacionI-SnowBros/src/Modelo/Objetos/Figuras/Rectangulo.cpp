#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){}

Rectangulo::Rectangulo(int x, int y, int ancho, int alto, std::string color, int angulo, float masa, bool estatico, b2World* world){
	this->x = x;
	this->y = y;
	this->alto = alto;
	this->ancho = ancho;
	this->masa = masa;
	this->angulo = angulo;
	this->estatico = estatico;
	this->color = color;
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_dynamicBody : cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del personaje
	b2PolygonShape shape;
	shape.SetAsBox(1,1);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = masa / (x * y);
	body->CreateFixture(&fixture);
}
