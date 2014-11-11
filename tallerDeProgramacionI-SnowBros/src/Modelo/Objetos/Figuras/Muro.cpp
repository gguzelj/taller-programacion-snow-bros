#include "../../../../headers/Modelo/Objetos/Figuras/Muro.h"

Muro::Muro(float ancho_esc, float alto_esc, int rot, b2World* world, float pos_x){

	if(rot == 0){
		this->y = -alto_esc/2;
		this->ancho = ancho_esc/2;
	}
	else{
		this->y = 0;
		this->ancho = alto_esc/2;
	}
	this->x = pos_x;
	this->angulo = rot * DEGTORAD;
	this->estatico = true;
	this->world = world;
	this->type = ID_MURO;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del muro
	b2PolygonShape shape;
	shape.SetAsBox(ancho,ALTO_CERO);
	b2FixtureDef fixtureDef;					//creo el fixture
	fixtureDef.shape = &shape;					//le asigno la forma que determine antes
	fixtureDef.density = 1;
	fixtureDef.friction = 0.5f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

Muro::~Muro(){
	this->world->DestroyBody(this->body);
}
