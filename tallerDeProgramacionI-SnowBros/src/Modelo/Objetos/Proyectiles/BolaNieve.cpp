#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaNieve.h"

BolaNieve::BolaNieve(float x, float y, b2World* world) {
	 this->type = "bolaNieve";

	 this->x = x;
	 this->y = y;
	 this->radio = RADIO_BOLA_NIEVE;
	 this->masa = MASA_BOLA_NIEVE*5;
	 this->angulo = 0;
	 this->estatico = false;
	 this->world = world;

	 //Defino el body y fixture
	 b2BodyDef cuerpoDeCirculo;
	 cuerpoDeCirculo.type = b2_dynamicBody;
	 cuerpoDeCirculo.position.Set(x,y);
	 cuerpoDeCirculo.gravityScale = 6;
	 this->body = this->world->CreateBody(&cuerpoDeCirculo);


	 b2FixtureDef fixture;
	 fixture.density = masa / (radio*radio*M_PI);

	 b2CircleShape circleShape;
	 circleShape.m_p.Set(0,0);
	 circleShape.m_radius = radio;

	 fixture.shape = &circleShape;
	 fixture.friction = 1.0f;
	 this->body->CreateFixture(&fixture);

}

BolaNieve::~BolaNieve() {

}

void BolaNieve::setVelocidad(b2Vec2 velocidad){
	this->body->SetLinearVelocity(velocidad);
}

float BolaNieve::getRadio(){
        return radio;
}

void BolaNieve::reaccionarCon(Figura* figura){
	figura->reaccionarConBolaNieve(this);
}
