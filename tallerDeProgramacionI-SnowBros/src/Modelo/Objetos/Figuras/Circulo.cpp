#include "../../../../headers/Modelo/Objetos/Figuras/Circulo.h"

Circulo::Circulo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->radio = parser->getEscalaObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	//Defino el body del circulo
	b2BodyDef cuerpoDeCirculo;
	estatico ? cuerpoDeCirculo.type = b2_staticBody : cuerpoDeCirculo.type = b2_dynamicBody; //veo que tipo le asigno dependiendo de que se pida
	cuerpoDeCirculo.angle = angulo;
	cuerpoDeCirculo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpoDeCirculo);
	this->body = body;

	//definiendo ahora el fixture del circulo
	b2CircleShape circleShape;
	circleShape.m_radius = radio;
	b2FixtureDef fixture;				//creo el fixture
	fixture.shape = &circleShape;		//le asigno la forma que determine antes
	fixture.density = masa / (radio*radio*M_PI);
	body->CreateFixture(&fixture);
}

Circulo::~Circulo(){
	this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}

float Circulo::getRadio(){
	return radio;
}

float Circulo::getCoordX(){
	b2Vec2 vec = this->body->GetPosition();
	return vec.x;
}

float Circulo::getCoordY(){
	b2Vec2 vec = this->body->GetPosition();
	return vec.y;
}


