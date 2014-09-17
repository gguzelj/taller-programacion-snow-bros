#include "../../../headers/Modelo/Objetos/Personaje.h"

Personaje::Personaje(JsonParser *parser, b2World* world){
	this->world = world;
	this->aceleracion = 10.0f;
	this->estaSaltando = false;
	this->x = parser->getCoorXPersonaje();
	this->y = parser->getCoorYPersonaje();

	this->contactos = new Contacto(this);
	this->world->SetContactListener(&contactos); 	//Logro sobrescribir el b2ContactListener
	this->body->SetUserData(this);

	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody;                       //El personaje sera un cuerpo dinamico
	cuerpoDelPersonaje.angle = 0;                                   //Angulo 0 asi empieza recto
	cuerpoDelPersonaje.position.Set(x,y);                           //donde queremos que empiece
	this->body = this->world->CreateBody(&cuerpoDelPersonaje);
	this->body->SetFixedRotation(true);		//Evito que rote

	//definiendo ahora el fixture del personaje
	b2PolygonShape formaDelPersonaje;						//esto va a ser si queremos que sea una caja...
	formaDelPersonaje.SetAsBox(0.5,1);						//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;						//creo el fixture
	fixtureDelPersonaje.shape = &formaDelPersonaje;			//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;						//una densidad cualquiera
	fixtureDelPersonaje.friction = 1;						//Le invento una friccion
	body->CreateFixture(&fixtureDelPersonaje);
/*
	//sensor tiene que ir al final porque meti un sensor
	formaDelPersonaje.SetAsBox(1, 2, b2Vec2(0,-3), 0);	//Le agrego otro fixture al personaje que va a servir como sensor, notar que esta muy relacionado con el tamaño del personaje, si se cambia cambiar esto tambien
	fixtureDelPersonaje.isSensor = true;					//Notar que ahora el fixture nuevo es sensor
	b2Fixture* fixtureDelSensor = body->CreateFixture(&fixtureDelPersonaje);
	fixtureDelSensor->SetUserData( (void*)DE_PIES );		//Le digo que es el pie
*/
}

Personaje::~Personaje(){
	//una posible destruccion del personaje
	this->world->DestroyBody(this->body);
}

void Personaje::moveLeft(){
	if (!estaSaltando){
    	b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
    	velocidadActual.x = -aceleracion;
    	this->body->SetLinearVelocity( velocidadActual );
	}
}

void Personaje::moveRight(){
	if (!estaSaltando){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
	}
}

void Personaje::jump(){
	  if (!estaSaltando){
		 float potenciaDeSalto = this->body->GetMass() * 10;
		 this->body->ApplyLinearImpulse( b2Vec2(0,potenciaDeSalto), this->body->GetWorldCenter(),true ); //Se podria cambiar el this->body->GetWorldCenter() por this->body->GetLocalCenter()
		 this->estaSaltando = true;
	  }
}

void Personaje::stop(){
	b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
	velocidadActual.x = 0.0f;
	this->body->SetLinearVelocity( velocidadActual );
}

void Personaje::startContact(){
	 this->estaSaltando = false;
}

void Personaje::endContact(){
	 this->estaSaltando = true;
}

b2Fixture* Personaje::GetFixtureList(){
	return this->body->GetFixtureList();
}


b2Vec2 Personaje::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}
