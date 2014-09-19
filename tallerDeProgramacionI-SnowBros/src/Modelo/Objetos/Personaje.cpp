#include "../../../headers/Modelo/Objetos/Personaje.h"

Personaje::Personaje(JsonParser *parser, b2World* world){
	//Parametros generales
	this->world = world;
	this->aceleracion = 10.0f;
	this->estaSaltando = false;
	this->x = parser->getCoorXPersonaje();
	this->y = parser->getCoorYPersonaje();

	//Parametros para controlar los contactos
	this->cantidadDeContactosActuales = 0; //Comienza en el aire
	this->contactos.updateContacto(&cantidadDeContactosActuales);
	this->world->SetContactListener(&contactos);

	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody;
	cuerpoDelPersonaje.angle = 0;
	cuerpoDelPersonaje.position.Set(x,y);
	cuerpoDelPersonaje.gravityScale = 4;

	//definiendo ahora el fixture del personaje
	float ancho = 0.5;
	float alto = 1;
	b2PolygonShape shapeDelPersonaje;						//esto va a ser si queremos que sea una caja...
	shapeDelPersonaje.SetAsBox(ancho,alto);						//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;						//creo el fixture
	fixtureDelPersonaje.shape = &shapeDelPersonaje;			//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;						//una densidad cualquiera
	fixtureDelPersonaje.friction = 1;						//Le invento una friccion

	this->body = this->world->CreateBody(&cuerpoDelPersonaje);
	this->body->SetFixedRotation(true);		//Evito que rote
	b2Fixture* fixture = body->CreateFixture(&fixtureDelPersonaje);

	//add foot sensor fixture beneath the main fixture
	shapeDelPersonaje.SetAsBox(ancho*9/10,alto/10,b2Vec2(0,-alto),0);
	fixtureDelPersonaje.isSensor = true;
	b2Fixture* footSensorFixture = this->body->CreateFixture(&fixtureDelPersonaje);
	footSensorFixture->SetUserData( (void*)ID_FOOT_SENSOR );

}

Personaje::~Personaje(){
	//una posible destruccion del personaje
	this->world->DestroyBody(this->body);
}

void Personaje::moveLeft(){
    	b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
    	velocidadActual.x = -aceleracion;
    	this->body->SetLinearVelocity( velocidadActual );
}

void Personaje::moveRight(){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
}

void Personaje::jump(){
	  if (cantidadDeContactosActuales!=0){
		 float potenciaDeSalto = this->body->GetMass() * 18;
		 this->body->ApplyLinearImpulse( b2Vec2(0,potenciaDeSalto), this->body->GetWorldCenter(),true ); //Se podria cambiar el this->body->GetWorldCenter() por this->body->GetLocalCenter()
	  }
}

void Personaje::stop(){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
		velocidadActual.x = velocidadActual.x * 0.17f;
		this->body->SetLinearVelocity( velocidadActual );
}

b2Fixture* Personaje::GetFixtureList(){
	return this->body->GetFixtureList();
}


b2Vec2 Personaje::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}
