#include "../../../headers/Modelo/Objetos/Enemigo.h"
#define ORIENTACION_INICIAL 'l'

Enemigo::Enemigo(float x, float y, b2World* world){
	//Parametros generales
	this->jumpCooldown = 0;
	this->world = world;
	this->aceleracion = 7.0f;
	this->x = x;
	this->y = y;
	//this->state = &Enemigo::standby;
	this->state = nullptr;
	this->orientacion = ORIENTACION_INICIAL;
	this->movimientoLateralDerecha = false;
	this->movimientoLateralIzquierda = false;
	this->debeSaltar = false;

	//Parametros para controlar los contactos
	this->cantidadDeContactosActuales = 0; //Comienza en el aire
	this->cantidadDeContactosIzquierda = 0;
	this->cantidadDeContactosDerecha = 0;

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
	fixtureDelEnemigo.filter.groupIndex = -1;				//Setting the groupIndex to negative will cause no collision

	this->body = this->world->CreateBody(&cuerpoDelEnemigo);
	this->body->SetFixedRotation(true);		//Evito que rote
	body->CreateFixture(&fixtureDelEnemigo);

	body->SetLinearDamping(0.5);

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
	footSensorFixture->SetUserData( (void*)ID_FOOT_SENSOR );

	//Sensor de izquierda y derecha
	shapeDelEnemigo.SetAsBox(0.0001f, alto-0.4f, b2Vec2(-ancho-0.9f,0.9f),0);
	fixtureDelEnemigo.isSensor = true;
	this->body->CreateFixture(&fixtureDelEnemigo);
	paredIzquierda->SetUserData((void*)ID_LEFT_WALL_SENSOR);
	shapeDelEnemigo.SetAsBox(0.0001f, alto-0.4f, b2Vec2(ancho+0.9f,0.9f),0);
	fixtureDelEnemigo.isSensor = true;
	this->body->CreateFixture(&fixtureDelEnemigo);
	paredDerecha->SetUserData((void*)ID_RIGHT_WALL_SENSOR);
}

Enemigo::~Enemigo(){
	this->world->DestroyBody(this->body);
}

void Enemigo::moveLeft(){
	if(this->cantidadDeContactosIzquierda == 0){
    	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
    	velocidadActual.x = -aceleracion;
    	this->body->SetLinearVelocity( velocidadActual );
	};
}

void Enemigo::moveRight(){
	if (this->cantidadDeContactosDerecha == 0){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
	};
}

void Enemigo::jump(){
	if (this->jumpCooldown <= 0){
		 this->jumpCooldown = 18;
			b2Vec2 velocidadActual = this->body->GetLinearVelocity();
			velocidadActual.y = 18;
			this->body->SetLinearVelocity( velocidadActual );
	};
}

b2Fixture* Enemigo::GetFixtureList(){
	return this->body->GetFixtureList();
}

b2Vec2 Enemigo::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}

void Enemigo::setOrientacion(char orientacion){
	this->orientacion = orientacion;
}

int Enemigo::getCantidadDeContactosActuales(){
	return this->cantidadDeContactosActuales;
}

b2Vec2 Enemigo::getVelocity(){
	return this->body->GetLinearVelocity();
}
/* Ojo con esto. Lo comento en el mail.
void Enemigo::handleInput(SDL_Keycode input,Uint32 input_type){
	this->state->handleInput(*this,input,input_type);
}*/

float Enemigo::getX(){
	return (this->body->GetPosition().x);
}

float Enemigo::getY(){
	return (this->body->GetPosition().y);
}

void Enemigo::decreaseJumpCooldown(){
	if (this->jumpCooldown > 0)
	this->jumpCooldown -= 1;
}

void Enemigo::updateLeftContact(int numero){
	this->cantidadDeContactosIzquierda = numero;
}

void Enemigo::updateRightContact(int numero){
	this->cantidadDeContactosDerecha = numero;
}
