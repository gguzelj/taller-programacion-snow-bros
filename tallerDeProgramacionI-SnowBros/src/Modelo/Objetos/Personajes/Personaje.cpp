#include "../../../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../../../headers/Modelo/Escenario.h"

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
	this->type = "personaje";

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
	this->body->SetFixedRotation(true);
	b2Fixture* fix = body->CreateFixture(&fixtureDelPersonaje);

	fix->SetUserData(this);
/*
	body->SetLinearDamping(0.5);

	//Partes laterales para que se deslice por las paredes y no se pegue a ellas
//	fixtureDelPersonaje.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	b2Fixture* paredIzquierda = this->body->CreateFixture(&fixtureDelPersonaje);
	b2Fixture* paredDerecha = this->body->CreateFixture(&fixtureDelPersonaje);

	//fixtureDelPersonaje.friction = 0.0019f;		//Le invento de 0 para que no se pegue a las paredes
	//this->body->CreateFixture(&fixtureDelPersonaje);

	b2Fixture* footSensorFixture = this->body->CreateFixture(&fixtureDelPersonaje);
	footSensorFixture->SetUserData((void*)ID_FOOT_SENSOR);

	//this->body->CreateFixture(&fixtureDelPersonaje);
	paredIzquierda->SetUserData((void*)ID_LEFT_WALL_SENSOR);

	this->body->CreateFixture(&fixtureDelPersonaje);
	paredDerecha->SetUserData((void*)ID_RIGHT_WALL_SENSOR);*/

}

Personaje::~Personaje() {
	this->world->DestroyBody(this->body);
}

void Personaje::disparar() {

	BolaNieve *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaNieve(getX() - 1, getY() + MITAD_ALTO_PERSONAJE - 0.40, this->world);
	else
		bola = new BolaNieve(getX() + 1, getY() + MITAD_ALTO_PERSONAJE - 0.40, this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	if (orientacion == IZQUIERDA)
		vel.x -= aceleracion * 2.0;
	else
		vel.x += aceleracion * 2.0;
	vel.y += 10;

	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

}

void Personaje::handleInput(SDL_Keycode input, Uint32 input_type) {
	this->state->handleInput(*this, input, input_type);
}

/*
 * Aca definimos como reacciona el personaje ante el contacto con el
 * enemigo.
 * El enemigo se pasa por parametro para que se pueda definir su comportamiento tambien
 */
void Personaje::reaccionarConEnemigo(Enemigo* enemigo){
	sacarVida();

	enemigo->type;
	//enemigo->sumarPuntos(???);
}
