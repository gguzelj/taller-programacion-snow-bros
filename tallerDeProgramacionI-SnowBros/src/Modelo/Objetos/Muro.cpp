#include "../../../headers/Modelo/Objetos/Muro.h"

Muro::Muro(float ancho_esc, float alto_esc, int rot, b2World* world, int pos_x){
	if(rot == 0){
		this->y = - (3.0f/7.0f)*alto_esc;
		this->ancho = ancho_esc/2;
	}
	else{
		this->y = 0;
		this->ancho = ancho_esc/2;
	}
	this->x = pos_x;
	this->alto = 0;
	this->angulo = rot * DEGTORAD;
	this->estatico = true;
	this->color = "#111111";
	this->world = world;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho,alto);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = 1;
	body->CreateFixture(&fixture);

}

Muro::~Muro(){

}
