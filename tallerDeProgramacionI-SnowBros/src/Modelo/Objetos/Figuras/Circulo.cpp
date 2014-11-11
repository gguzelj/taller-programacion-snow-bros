#include "../../../../headers/Modelo/Objetos/Figuras/Circulo.h"

Circulo::Circulo(JsonParser *parser, int index, b2World* world){
	this->type = ID_CIRCULO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->radio = parser->getEscalaObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	estatico ? cuerpoDeCirculo.type = b2_staticBody : cuerpoDeCirculo.type = b2_dynamicBody; //veo que tipo le asigno dependiendo de que se pida
	cuerpoDeCirculo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpoDeCirculo);

	b2FixtureDef fixtureDef;                          //creo el fixture
	fixtureDef.density = masa / (radio*radio*M_PI);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0,0);
	circleShape.m_radius = radio;

	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 1.0f;

	b2Fixture* fixture = this->body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
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

float Circulo::getAlto(){
	return radio * 2;
}
float Circulo::getAncho(){
	return radio * 2;
}
char Circulo::getId(){
	return CIRCULO_CODE;
}
