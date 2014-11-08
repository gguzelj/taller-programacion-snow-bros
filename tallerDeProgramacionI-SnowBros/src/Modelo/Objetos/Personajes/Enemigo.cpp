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

	//Parametros para controlar los contactos
	this->contactosActuales = 0; //Comienza en el aire
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

	//definiendo el body del personaje
	b2BodyDef cuerpoDelEnemigo;
	cuerpoDelEnemigo.type = b2_dynamicBody;
	cuerpoDelEnemigo.angle = 0;
	cuerpoDelEnemigo.position.Set(x,y);
	cuerpoDelEnemigo.gravityScale = 4;

	//definiendo ahora el fixture del personaje
	this->ancho = MITAD_ANCHO_ENEMIGO;
	this->alto = MITAD_ALTO_ENEMIGO;

	b2PolygonShape shapeDelEnemigo;						//esto va a ser si queremos que sea una caja...
	shapeDelEnemigo.SetAsBox(ancho,alto);					//...con las siguientes dimensiones
	b2FixtureDef fixtureDelEnemigo;						//creo el fixture
	fixtureDelEnemigo.shape = &shapeDelEnemigo;			//le asigno la forma que determine antes
	fixtureDelEnemigo.density = 1;						//una densidad cualquiera
	fixtureDelEnemigo.friction = 1;						//Le invento una friccion

	this->body = this->world->CreateBody(&cuerpoDelEnemigo);
	this->body->SetFixedRotation(true);
	b2Fixture* fix = body->CreateFixture(&fixtureDelEnemigo);

	fix->SetUserData(this);


	body->SetLinearDamping(0.5);
/*
	//Partes laterales para que se deslice por las paredes y no se pegue a ellas
	shapeDelEnemigo.SetAsBox(0.0000001f, alto-0.00405f, b2Vec2(-ancho+0.00000005,0.0045f),0);	//a la izquierda
	fixtureDelEnemigo.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	b2Fixture* paredIzquierda = this->body->CreateFixture(&fixtureDelEnemigo);
	shapeDelEnemigo.SetAsBox(0.0000001f, alto-0.00405f, b2Vec2(ancho-0.00000005,0.0045f),0);	//a la derecha
	b2Fixture* paredDerecha = this->body->CreateFixture(&fixtureDelEnemigo);

	shapeDelEnemigo.SetAsBox(ancho, 0.0001f, b2Vec2(0,-alto),0);	//a la izquierda
	fixtureDelEnemigo.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	this->body->CreateFixture(&fixtureDelEnemigo);

	//Foot sensor
	shapeDelEnemigo.SetAsBox(ancho*19.5f/20,alto/10,b2Vec2(0,-alto),0);
	fixtureDelEnemigo.isSensor = true;
	b2Fixture* footSensorFixture = this->body->CreateFixture(&fixtureDelEnemigo);
	footSensorFixture->SetUserData(this);

	//Sensor de izquierda y derecha
	shapeDelEnemigo.SetAsBox(0.0001f, alto - 0.4f, b2Vec2(-ancho - 0.9f, 0.9f), 0);
	fixtureDelEnemigo.isSensor = true;
	this->body->CreateFixture(&fixtureDelEnemigo);
	paredIzquierda->SetUserData(this);
	shapeDelEnemigo.SetAsBox(0.0001f, alto - 0.4f, b2Vec2(ancho + 0.9f, 0.9f), 0);
	fixtureDelEnemigo.isSensor = true;
	this->body->CreateFixture(&fixtureDelEnemigo);
	paredDerecha->SetUserData(this);
	*/
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

}