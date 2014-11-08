#include "../../../../headers/Modelo/Objetos/Personajes/Enemigo.h"
#define ORIENTACION_INICIAL 'l'

Enemigo::Enemigo(float x, float y, b2World* world){
	//Parametros generales
	this->jumpCooldown = 0;
	this->world = world;
	this->aceleracion = 7.0f;
	this->x = x;
	this->y = y;
	this->state = &Enemigo::standby;
	this->state = nullptr;
	this->orientacion = ORIENTACION_INICIAL;
	this->movimientoLateralDerecha = false;
	this->movimientoLateralIzquierda = false;
	this->debeSaltar = false;
	this->type = "ENEMIGO";
	this->lives = 5;
	this->ancho = MITAD_ANCHO_ENEMIGO;
	this->alto = MITAD_ALTO_ENEMIGO;

	//Parametros para controlar los contactos
	this->contactosActuales = 0;
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

	//Defino body del Enemigo
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angle = 0;
	bodyDef.position.Set(x,y);
	bodyDef.gravityScale = 4;
	this->body = this->world->CreateBody(&bodyDef);

	//Defino el Shape del enemigo
	b2PolygonShape shapeDelEnemigo;
	shapeDelEnemigo.SetAsBox(ancho, alto);

	//Definimos el fixture del Enemigo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDelEnemigo;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelEnemigo.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(-ancho + 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelEnemigo.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(ancho - 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelEnemigo.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);
	fixtureDef.friction = 0.0019f;
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

}

Enemigo::~Enemigo(){
	this->world->DestroyBody(this->body);
}

void Enemigo::disparar(){

}

void Enemigo::handleInput(SDL_Keycode input,Uint32 input_type){
		(this->state)->handleInput(*this,input,input_type);
}

void Enemigo::reaccionarCon(Figura* figura){
	figura->reaccionarConEnemigo(this);
}

void Enemigo::reaccionarConBolaNieve(BolaNieve* bola){


	//IF CORRESPONDE CONGELAR=>
	std::thread t(&Enemigo::congelar, this);
	t.detach();

}


void Enemigo::congelar(){

	aceleracion = 0;

	sleep(5);

	aceleracion = 7.0f;

}
