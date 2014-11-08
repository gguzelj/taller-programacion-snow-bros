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
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	//Parametros para controlar los contactos
	this->contactosActuales = 0;
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

	//Defino body del personaje
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angle = 0;
	bodyDef.position.Set(x, y);
	bodyDef.gravityScale = 4;
	this->body = this->world->CreateBody(&bodyDef);

	//Defino el Shape del personaje
	b2PolygonShape shapeDelPersonaje;
	shapeDelPersonaje.SetAsBox(ancho, alto);

	//Definimos el fixture del personaje
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDelPersonaje;
	fixtureDef.density = 1;
	fixtureDef.filter.groupIndex = -1;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(-ancho + 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f, b2Vec2(ancho - 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelPersonaje.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);
	fixtureDef.friction = 0.0019f;
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

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
void Personaje::reaccionarConEnemigo(Enemigo* enemigo) {
	sacarVida();

	enemigo->type;
	//enemigo->sumarPuntos(???);
}

void cambiarEstadoAlAterrizar(Character* character) {
	if (character->movimientoLateralDerecha == true || character->movimientoLateralIzquierda == true)
		character->state = &Character::walking;
	else
		character->state = &Character::standby;
}

void Personaje::empiezoContacto(b2Fixture* fixture) {

	//contacto con derecha?
	if (paredDerecha == fixture)
		contactosDerecha++;

	//Contacto con izquierda?
	if (paredIzquierda == fixture)
		contactosIzquierda++;

	//Contacto con Piso?
	if (fixture == piso) {

		if (contactosActuales == 0)
			cambiarEstadoAlAterrizar(this);

		contactosActuales++;

	}

}

void Personaje::terminoContacto(b2Fixture* fixture) {

	if (paredDerecha == fixture)
		contactosDerecha--;

	if (paredIzquierda == fixture)
		contactosIzquierda--;

	if (piso == fixture) {

		contactosActuales--;

		if (contactosActuales == 0 && state->getCode() != JUMPING)
			state = &Personaje::falling;

	}

}
