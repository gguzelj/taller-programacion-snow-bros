#include "../../../headers/Modelo/Objetos/Personaje.h"
#define ORIENTACION_INICIAL 'l'

Personaje::Personaje(float x, float y,conn_id id, b2World* world){
	//Parametros generales
	this->jumpCooldown = 0;
	this->world = world;
	this->aceleracion = 10.0f;
	this->x = x;
	this->y = y;
	strcpy(this->id,id);
	this->state = &Personaje::standby;
	this->orientacion = ORIENTACION_INICIAL;
	this->walking.movimientoLateralDerecha = false;
	this->walking.movimientoLateralIzquierda = false;
	this->connectionState = CONECTADO;

	//Parametros para controlar los contactos
	//this->contactos.setPersonaje(this);
	this->cantidadDeContactosActuales = 0; //Comienza en el aire
	this->cantidadDeContactosIzquierda = 0;
	this->cantidadDeContactosDerecha = 0;
	//this->contactos.updateContacto(&cantidadDeContactosActuales,&cantidadDeContactosIzquierda,&cantidadDeContactosDerecha);
	//this->world->SetContactListener(&contactos);


	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody;
	cuerpoDelPersonaje.angle = 0;
	cuerpoDelPersonaje.position.Set(x,y);
	cuerpoDelPersonaje.gravityScale = 4;

	//definiendo ahora el fixture del personaje
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	b2PolygonShape shapeDelPersonaje;						//esto va a ser si queremos que sea una caja...
	shapeDelPersonaje.SetAsBox(ancho,alto);		//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;						//creo el fixture
	fixtureDelPersonaje.shape = &shapeDelPersonaje;			//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;						//una densidad cualquiera
	fixtureDelPersonaje.friction = 1;						//Le invento una friccion

	this->body = this->world->CreateBody(&cuerpoDelPersonaje);
	this->body->SetFixedRotation(true);		//Evito que rote
	body->CreateFixture(&fixtureDelPersonaje);

	//Partes laterales para que se deslice por las paredes y no se pegue a ellas
	shapeDelPersonaje.SetAsBox(0.0000001f, alto-0.004f, b2Vec2(-ancho+0.00000005,0.0045f),0);	//a la izquierda
	fixtureDelPersonaje.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	b2Fixture* paredIzquierda = this->body->CreateFixture(&fixtureDelPersonaje);
	shapeDelPersonaje.SetAsBox(0.0000001f, alto-0.004f, b2Vec2(ancho-0.00000005,0.0045f),0);	//a la derecha
	b2Fixture* paredDerecha = this->body->CreateFixture(&fixtureDelPersonaje);

	//Foot sensor
	shapeDelPersonaje.SetAsBox(ancho*19.0f/20,alto/10,b2Vec2(0,-alto),0);
	fixtureDelPersonaje.isSensor = true;
	b2Fixture* footSensorFixture = this->body->CreateFixture(&fixtureDelPersonaje);
	footSensorFixture->SetUserData( (void*)ID_FOOT_SENSOR );

	//Sensor de izquierda y derecha
	shapeDelPersonaje.SetAsBox(0.0001f, alto-0.4f, b2Vec2(-ancho-0.9f,0.9f),0);
	fixtureDelPersonaje.isSensor = true;
	this->body->CreateFixture(&fixtureDelPersonaje);
	paredIzquierda->SetUserData((void*)ID_LEFT_WALL_SENSOR);
	shapeDelPersonaje.SetAsBox(0.0001f, alto-0.4f, b2Vec2(ancho+0.9f,0.9f),0);
	fixtureDelPersonaje.isSensor = true;
	this->body->CreateFixture(&fixtureDelPersonaje);
	paredDerecha->SetUserData((void*)ID_RIGHT_WALL_SENSOR);

}

Personaje::~Personaje(){
	this->world->DestroyBody(this->body);
}

void Personaje::moveLeft(){
	if(this->cantidadDeContactosIzquierda == 0){
    	b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
    	velocidadActual.x = -aceleracion;
    	this->body->SetLinearVelocity( velocidadActual );
	};
}

void Personaje::moveRight(){
	if (this->cantidadDeContactosDerecha == 0){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
	};
}

void Personaje::jump(){
	if (this->jumpCooldown <= 0){
		 this->jumpCooldown = 18;
		 float potenciaDeSalto = this->body->GetMass() * 18;
		 this->body->ApplyLinearImpulse( b2Vec2(0,potenciaDeSalto), this->body->GetWorldCenter(),true );
	};
}

void Personaje::stop(){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
		velocidadActual.x = velocidadActual.x * 0;
		this->body->SetLinearVelocity( velocidadActual );
}

b2Fixture* Personaje::GetFixtureList(){
	return this->body->GetFixtureList();
}


b2Vec2 Personaje::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}


char Personaje::getOrientacion(){
	return this->orientacion;
}

void Personaje::setOrientacion(char orientacion){
	this->orientacion = orientacion;
}

int Personaje::getCantidadDeContactosActuales(){

	return this->cantidadDeContactosActuales;
}

b2Vec2 Personaje::getVelocity(){
	return this->body->GetLinearVelocity();
}


void Personaje::handleInput(SDL_Keycode input,Uint32 input_type){
	this->state->handleInput(*this,input,input_type);
}

float Personaje::getX(){
	return (this->body->GetPosition().x);
}

float Personaje::getY(){
	return (this->body->GetPosition().y);
}

void Personaje::decreaseJumpCooldown(){
	if (this->jumpCooldown > 0)
	this->jumpCooldown -= 1;
}

int Personaje::getJumpCooldown(){
	return (this->jumpCooldown);
}

void Personaje::updateLeftContact(int numero){
	this->cantidadDeContactosIzquierda = numero;
}

void Personaje::updateRightContact(int numero){
	this->cantidadDeContactosDerecha = numero;
}

void Personaje::printContactosLR(){
	printf("contactosLEFT %d\n", this->cantidadDeContactosIzquierda);
	printf("contactosRIGHT %d\n", this->cantidadDeContactosDerecha);
}
