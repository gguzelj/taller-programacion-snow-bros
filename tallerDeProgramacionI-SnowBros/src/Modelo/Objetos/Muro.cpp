#include "../../../headers/Modelo/Objetos/Muro.h"

Muro::Muro(float ancho_esc, float alto_esc, int rot, b2World* world, float pos_x){
<<<<<<< HEAD
=======
	float friction = 0;
>>>>>>> refs/remotes/origin/movimientoDePersonaje
	if(rot == 0){
		this->y = -alto_esc/2;
		this->ancho = ancho_esc/2;
<<<<<<< HEAD
=======
		friction = 0.7f;
>>>>>>> refs/remotes/origin/movimientoDePersonaje
	}
	else{
		this->y = 0;
		this->ancho = alto_esc/2;
	}
	this->x = pos_x;
	this->angulo = rot * DEGTORAD;
	this->estatico = true;
	this->world = world;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho,ALTO_CERO);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = 1;
	fixture.friction = 0.5f;
	body->CreateFixture(&fixture);
}

Muro::~Muro(){
	this->world->DestroyBody(this->body);
}
