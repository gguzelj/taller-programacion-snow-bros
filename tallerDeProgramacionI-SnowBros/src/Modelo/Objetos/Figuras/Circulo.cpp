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

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	estatico ? cuerpoDeCirculo.type = b2_staticBody : cuerpoDeCirculo.type = b2_dynamicBody; //veo que tipo le asigno dependiendo de que se pida
	b2FixtureDef fixture1;                          //creo el fixture
	fixture1.density = masa / (radio*radio*M_PI);
	b2FixtureDef fixture2;                          //creo el fixture
	fixture2.density = 0.000000001;
	cuerpoDeCirculo.position.Set(x,y);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0,0);
	circleShape.m_radius = radio;

	fixture1.shape = &circleShape;
	fixture1.friction = 0.1f;
	this->body = this->world->CreateBody(&cuerpoDeCirculo);
	this->body->CreateFixture(&fixture1);

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
