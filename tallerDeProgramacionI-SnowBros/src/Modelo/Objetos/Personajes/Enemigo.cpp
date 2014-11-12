#include "../../../../headers/Modelo/Objetos/Personajes/Enemigo.h"
#include <ctime>
#define ORIENTACION_INICIAL 'l'

Enemigo::Enemigo(float x, float y, b2World* world) {
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
	this->tiempoDeImpactoDeLaUltimaBola = 0.0f;
	this->debeSaltar = false;
	this->esAtravezable = false;
	this->type = ID_ENEMIGO;
	this->lives = 5;
	this->ancho = MITAD_ANCHO_ENEMIGO;
	this->alto = MITAD_ALTO_ENEMIGO;
	this->nivelDeCongelamiento = 0;
	this->puedeEmpujar = false;
	this->estaVivo = true;
	this->enMovimientoBola = false;
	this->puntos = 50;

	//Parametros para controlar los contactos
	this->contactosActuales = 0;
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

	//Defino body del Enemigo
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angle = 0;
	bodyDef.position.Set(x, y);
	bodyDef.gravityScale = 4;
	this->body = this->world->CreateBody(&bodyDef);

	//Defino el Shape del enemigo
	b2PolygonShape shapeDelEnemigo;
	shapeDelEnemigo.SetAsBox(ancho, alto);

	//Definimos el fixture del Enemigo
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDelEnemigo;
	fixtureDef.density = 5;
	fixtureDef.filter.groupIndex = ENEMIGO_FILTER_INDEX;
	fixtureDef.friction = 1;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(-ancho + 0.01, 0), 0);
	fixtureDef.friction = 0.01f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelEnemigo.SetAsBox(0.01f, alto / 2, b2Vec2(ancho - 0.01, 0), 0);
	fixtureDef.friction = 0.01f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelEnemigo.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);;
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

void Enemigo::empujar(){

}

void Enemigo::morir(){
	enMovimientoBola = true;
//	std::thread t(&Enemigo::movimientoBola, this);
//	t.detach();
}
/*
void Enemigo::movimientoBola(){
	time_t tiempoDeCreacionDeBola;
	time(&tiempoDeCreacionDeBola);
	float tiempoDeEsperaMaximo = 5.0f;
	time_t newTime = time(&newTime);
	//Luego de 5 segs la bola se debe destruir
	while( difftime(newTime, tiempoDeImpactoDeLaUltimaBola )  < tiempoDeEsperaMaximo){
		newTime = time(&newTime);
	}
	estaVivo = false;
}
*/
void Enemigo::handleInput(SDL_Keycode input,Uint32 input_type){
	if(nivelDeCongelamiento > 0) return;
	state->handleInput(*this,input,input_type);
}

void Enemigo::reaccionarCon(Figura* figura){
	figura->reaccionarConEnemigo(this);
}

void Enemigo::reaccionarConBolaNieve(BolaNieve* bola){

	time(&tiempoDeImpactoDeLaUltimaBola);

	//Si estaba semicongelado, lo congelamos un poco mas
	if(this->nivelDeCongelamiento > 0 ){
		this->nivelDeCongelamiento += bola->potencia;
		if(this->nivelDeCongelamiento > NIVEL_CONGELAMIENTO_MAX)
			this->nivelDeCongelamiento = NIVEL_CONGELAMIENTO_MAX;
	}

	//Si no estaba congelado, empezamos a congelar
	if (this->nivelDeCongelamiento == 0){
		this->state = &Enemigo::standby;
		this->nivelDeCongelamiento += bola->potencia;
		std::thread t(&Enemigo::congelar, this);
		t.detach();
	}
}

void Enemigo::reaccionarConEnemigo(Enemigo* enemigo){
	if(enemigo->enMovimientoBola){
		estaVivo = false;
	}
}

void Enemigo::congelar(){
	float tiempoDeEsperaMaximo = 5.0f;

	while (nivelDeCongelamiento != 0 && !enMovimientoBola){
		//En caso de que este hecho bola de nieve, lo hacemos
		//No atravezable, para que pueda empujarlo
		esAtravezable = (nivelDeCongelamiento != NIVEL_CONGELAMIENTO_MAX);
		aceleracion = 0;
		if( difftime(time(nullptr), tiempoDeImpactoDeLaUltimaBola )  > tiempoDeEsperaMaximo){
			this->nivelDeCongelamiento -=2;
			if(this->nivelDeCongelamiento < 0)
				this->nivelDeCongelamiento = 0;
			time(&tiempoDeImpactoDeLaUltimaBola);
		}
		//En caso de que este hecho bola de nieve, lo hacemos
		//No atravezable, para que pueda empujarlo
		esAtravezable = (nivelDeCongelamiento != NIVEL_CONGELAMIENTO_MAX);
		aceleracion = 0;
	}
	esAtravezable = false;
	if (!enMovimientoBola)
		aceleracion = 7.0f;
	else
		aceleracion = 25.0f;
}

bool Enemigo::estaCongelado(){
	return (nivelDeCongelamiento == NIVEL_CONGELAMIENTO_MAX);
}

void Enemigo::jump(){
	if( this->nivelDeCongelamiento == 0){
		if (this->jumpCooldown <= 0) {
			this->jumpCooldown = 18;
			b2Vec2 velocidadActual = this->body->GetLinearVelocity();
			velocidadActual.y = 25;
			this->body->SetLinearVelocity(velocidadActual);
		}
	}
}


void Enemigo::hacerAtravezable() {
	this->cambiarFilterIndex(PERSONAJE_FILTER_INDEX);
}

void Enemigo::hacerNoAtravezable() {
	this->cambiarFilterIndex(ENEMIGO_FILTER_INDEX);
}
