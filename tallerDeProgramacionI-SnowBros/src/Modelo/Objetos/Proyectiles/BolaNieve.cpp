#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaNieve.h"

BolaNieve::BolaNieve(b2World* world) {
	 this->type = "bolaNieve";

	 this->x = 0;
	 this->y = 0;
	 this->radio = RADIO_BOLA_NIEVE;
	 this->masa = MASA_BOLA_NIEVE;
	 this->angulo = 0;
	 this->estatico = false;
	 this->world = world;

	 //Defino el body y fixture
	 b2BodyDef cuerpoDeCirculo;
	 cuerpoDeCirculo.type = b2_dynamicBody;
	 cuerpoDeCirculo.position.Set(x,y);
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
	this->world->DestroyBody(this->body);
}

void BolaNieve::setVelocidad(b2Vec2 velocidad){
	this->body->SetLinearVelocity(velocidad);
}

