#include "../../../../headers/Modelo/Objetos/Figuras/Circulo.h"

Circulo::Circulo(int posicionX, int posicionY, float radio, std::string color, int angulo, float masa, bool estatico, b2World* world){
	this->x = posicionX;
	this->y = posicionY;
	this->radio = radio;
	this->masa = masa;
	this->angulo = angulo;
	this->estatico = estatico;
	this->color = color;
	this->world = world;

	//Defino el body del circulo
	b2BodyDef cuerpoDeCirculo;
	estatico ? cuerpoDeCirculo.type = b2_dynamicBody : cuerpoDeCirculo.type = b2_staticBody; //veo que tipo le asigno dependiendo de que se pida
	cuerpoDeCirculo.angle = angulo;
	cuerpoDeCirculo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpoDeCirculo);
	this->body = body;

	//definiendo ahora el fixture del circulo
	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0); 			//posicion en offset del cuerpoDeCirculo.position.Set(x,y);
	circleShape.m_radius = radio;
	b2FixtureDef fixture;				//creo el fixture
	fixture.shape = &circleShape;		//le asigno la forma que determine antes
	fixture.density = masa / (radio*radio*3.141596);
	body->CreateFixture(&fixture);
}

Circulo::~Circulo(){
	this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}
