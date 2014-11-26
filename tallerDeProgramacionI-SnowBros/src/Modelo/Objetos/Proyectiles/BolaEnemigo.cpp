#include "../../../../headers/Modelo/Objetos/Proyectiles/BolaEnemigo.h"
#include "../../../../headers/Modelo/Escenario.h"

BolaEnemigo::BolaEnemigo(float x, float y, b2World* world, Escenario* esc) {
	this->type = ID_BOLA_NIEVE_ENEMIGO;
	this->x = x;
	this->y = y;
	this->radio = RADIO_BOLA_NIEVE_ENEMIGO;
	this->masa = MASA_BOLA_NIEVE_ENEMIGO;
	this->angulo = 0;
	this->estatico = false;
	this->destruir = false;
	this->teletransportar = false;
	this->world = world;
	this->escenario = esc;
	this->velocidad = {25,0};

	//Defino el body y fixture
	b2BodyDef cuerpoDeCirculo;
	cuerpoDeCirculo.type = b2_dynamicBody;
	cuerpoDeCirculo.position.Set(x, y);
	cuerpoDeCirculo.gravityScale = 15;
	this->body = this->world->CreateBody(&cuerpoDeCirculo);

	b2CircleShape circleShape;
	circleShape.m_p.Set(0, 0);
	circleShape.m_radius = radio;

	b2FixtureDef fixture;
	fixture.filter.groupIndex = BOLA_ENEMIGO_FILTER_INDEX;
	fixture.density = 10;
	fixture.shape = &circleShape;
	fixture.friction = 0;
	b2Fixture *fix = this->body->CreateFixture(&fixture);

	fix->SetUserData(this);

	//Defino el Shape del sensor
	b2PolygonShape shapeDelSensor;
	shapeDelSensor.SetAsBox(radio, radio);
	fixture.shape = &shapeDelSensor;
	//fixture.isSensor = true;

	//Pared Izquierda
	shapeDelSensor.SetAsBox(radio / 1000, radio* 0.002, b2Vec2(-radio, 0), 0);
	paredIzquierda = this->body->CreateFixture(&fixture);

	//ParedDerecha
	shapeDelSensor.SetAsBox(radio/ 1000, radio* 0.002, b2Vec2(radio, 0), 0);
	paredDerecha = this->body->CreateFixture(&fixture);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
}

BolaEnemigo::~BolaEnemigo() {
}

void BolaEnemigo::setVelocidad(b2Vec2 velocidad) {
	this->body->SetLinearVelocity(velocidad);
}

float BolaEnemigo::getRadio() {
	return radio;
}

float BolaEnemigo::getAlto() {
	return radio * 2;
}

float BolaEnemigo::getAncho() {
	return radio * 2;
}

char BolaEnemigo::getId() {
	return BOLA_NIEVE_CODE;
}

//Deprecated
void BolaEnemigo::actualizar(){
	if(this->getVelocidad().y <0)
		this->velocidad.y = this->getVelocidad().y;
	else
		this->velocidad.y = 0;
	if (this->getVelocidad().x > 0){
		this->velocidad.x = VELX;
	}
	else
		this->velocidad.x = -VELX;
	this->setVelocidad(velocidad);
}

void BolaEnemigo::morir(){
	time_t tiempoInicial, tiempoFinal;
	time(&tiempoInicial);
	time(&tiempoFinal);
	while(difftime(tiempoFinal,tiempoInicial) < 3){
		sleep(0.5);
		if(escenario->enStep())
			this->actualizar();
		time(&tiempoFinal);
	}
	escenario->agregarSonido(BALLBREAKING);
	if (escenario->debeCrearBonus())
		escenario->agregarBonus(escenario->crearBonus(this));
	destruir = true;
}

void BolaEnemigo::beginContact(Figura *fig, b2Contact* contact){
	b2Fixture* fix;
	this == contact->GetFixtureA()->GetUserData() ? fix = contact->GetFixtureA() : fix = contact->GetFixtureB();
	b2Vec2 vel = this->body->GetLinearVelocity();
	if(fix == paredDerecha && fig->esEstatico())
		vel = {-VELX,0};
	else if (fix == paredIzquierda && fig->esEstatico())
		vel = {VELX,0};
	this->body->SetLinearVelocity(vel);
	fig->beginContactBolaEnemigo(this, contact);
}

void BolaEnemigo::cambiarFilterIndex(int16 groupIndex){
	b2Filter filter;
	filter.groupIndex = groupIndex;

	//Recorremos todos los fixtures del objeto y cambiamos el filter
	for (b2Fixture* fix = this->body->GetFixtureList(); fix; fix = fix->GetNext()) {
		fix->SetFilterData(filter);
	}

}
