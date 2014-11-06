#include "../../../headers/Modelo/Objetos/Personaje.h"

#define ORIENTACION_INICIAL 'l'

Personaje::Personaje(float x, float y, conn_id id, Escenario* escenario) {
	//Parametros generales
	this->escenario_ = escenario;
	this->jumpCooldown = 0;
	this->world = escenario->getWorld();
	this->aceleracion = 10.0f;
	this->x = x;
	this->y = y;
	strcpy(this->id, id);
	this->state = &Personaje::standby;
	this->orientacion = ORIENTACION_INICIAL;
	this->movimientoLateralDerecha = false;
	this->movimientoLateralIzquierda = false;
	this->debeSaltar = false;
	this->connectionState = CONECTADO;
	this->points = 0;
	this->lives = 3;

	//Parametros para controlar los contactos
	this->cantidadDeContactosActuales = 0; //Comienza en el aire
	this->cantidadDeContactosIzquierda = 0;
	this->cantidadDeContactosDerecha = 0;

	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody;
	cuerpoDelPersonaje.angle = 0;
	cuerpoDelPersonaje.position.Set(x, y);
	cuerpoDelPersonaje.gravityScale = 4;

	//definiendo ahora el fixture del personaje
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	b2PolygonShape shapeDelPersonaje;				//esto va a ser si queremos que sea una caja...
	shapeDelPersonaje.SetAsBox(ancho, alto);					//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;						//creo el fixture
	fixtureDelPersonaje.shape = &shapeDelPersonaje;			//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;						//una densidad cualquiera
	fixtureDelPersonaje.friction = 1;						//Le invento una friccion
	fixtureDelPersonaje.filter.groupIndex = -1;	//Setting the groupIndex to negative will cause no collision

	this->body = this->world->CreateBody(&cuerpoDelPersonaje);
	this->body->SetFixedRotation(true);		//Evito que rote
	body->CreateFixture(&fixtureDelPersonaje);

	body->SetLinearDamping(0.5);

	//Partes laterales para que se deslice por las paredes y no se pegue a ellas
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(-ancho + 0.00000005, 0.0045f),
			0);	//a la izquierda
	fixtureDelPersonaje.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	b2Fixture* paredIzquierda = this->body->CreateFixture(&fixtureDelPersonaje);
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(ancho - 0.00000005, 0.0045f), 0);//a la derecha
	b2Fixture* paredDerecha = this->body->CreateFixture(&fixtureDelPersonaje);

	shapeDelPersonaje.SetAsBox(ancho, 0.0001f, b2Vec2(0, -alto), 0);	//a la izquierda
	fixtureDelPersonaje.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	this->body->CreateFixture(&fixtureDelPersonaje);

	//Foot sensor
	shapeDelPersonaje.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);
	fixtureDelPersonaje.isSensor = true;
	b2Fixture* footSensorFixture = this->body->CreateFixture(&fixtureDelPersonaje);
	footSensorFixture->SetUserData((void*) ID_FOOT_SENSOR);

	//Sensor de izquierda y derecha
	shapeDelPersonaje.SetAsBox(0.0001f, alto - 0.4f, b2Vec2(-ancho - 0.9f, 0.9f), 0);
	fixtureDelPersonaje.isSensor = true;
	this->body->CreateFixture(&fixtureDelPersonaje);
	paredIzquierda->SetUserData((void*) ID_LEFT_WALL_SENSOR);
	shapeDelPersonaje.SetAsBox(0.0001f, alto - 0.4f, b2Vec2(ancho + 0.9f, 0.9f), 0);
	fixtureDelPersonaje.isSensor = true;
	this->body->CreateFixture(&fixtureDelPersonaje);
	paredDerecha->SetUserData((void*) ID_RIGHT_WALL_SENSOR);
}

Personaje::~Personaje() {
	this->world->DestroyBody(this->body);
}

void Personaje::disparar() {

	BolaNieve *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaNieve(getX()-0, getY()+MITAD_ALTO_PERSONAJE+0.40, this->world);
	else
		bola = new BolaNieve(getX()+0, getY()+MITAD_ALTO_PERSONAJE+0.40, this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	if(orientacion == IZQUIERDA)
		vel.x -= aceleracion * 2.0 ;
	else
		vel.x += aceleracion * 2.0 ;
	vel.y += 10;

	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

}

void Personaje::handleInput(SDL_Keycode input, Uint32 input_type) {
        this->state->handleInput(*this, input, input_type);
}
