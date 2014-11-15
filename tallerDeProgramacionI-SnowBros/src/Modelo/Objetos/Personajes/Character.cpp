#include "../../../../headers/Modelo/Objetos/Personajes/Character.h"

/*
 *	Seteamos la orientacion del personaje y lo hacemos caminar
 */
void Character::move(char orient) {

	//Primero intentamos movermos en la direccion indicada
	//pero solamente si el personaje ya se estaba moviendo
	//en esa direccion
	if (movimientoIzquierda)
		return;

	if (movimientoDerecha)
		return;

	//Si el personaje no se movia para ningun lado, seteamos
	//una orientacion y lo movemos
	movimientoIzquierda = (orient == IZQUIERDA);
	movimientoDerecha = (orient == DERECHA);
	orientacion = orient;

	move(orient);
}

void Character::move() {

	if (movimientoIzquierda) {
		moveLeft();
		return;
	}

	if (movimientoDerecha) {
		moveRight();
		return;
	}

}

/*
 * Solo nos detenemos si la orientacion es la misma en que teniamos
 */
bool Character::detener(char orientacion) {

	if (movimientoIzquierda && (orientacion == IZQUIERDA)) {
		movimientoIzquierda = false;
		stop();
		return true;
	}

	if (movimientoDerecha && (orientacion == DERECHA)) {
		movimientoDerecha = false;
		stop();
		return true;
	}
	return false;
}

void Character::disparar() {
	this->movimientoDisparar = true;
}

void Character::dejarDisparar() {
	this->movimientoDisparar = false;
}

void Character::realizarDisparo() {
}

void Character::moveLeft() {
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.x = -aceleracion;
	this->body->SetLinearVelocity(velocidadActual);
}

void Character::moveRight() {
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.x = aceleracion;
	this->body->SetLinearVelocity(velocidadActual);
}

void Character::pushLeft() {
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.x = -aceleracion;
	this->body->SetLinearVelocity(velocidadActual);
}

void Character::pushRight() {
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.x = aceleracion;
	this->body->SetLinearVelocity(velocidadActual);
}

void Character::stop() {
	b2Vec2 velocidadActual = this->body->GetLinearVelocity();
	velocidadActual.x = 0;
	if (state == &Character::walking)
		velocidadActual.y = 0;
	this->body->SetLinearVelocity(velocidadActual);
}

b2Fixture* Character::GetFixtureList() {
	return this->body->GetFixtureList();
}

b2Vec2 Character::GetWorldPoint(const b2Vec2& localPoint) {
	return body->GetWorldPoint(localPoint);
}

char Character::getOrientacion() {
	return this->orientacion;
}

void Character::setOrientacion(char orientacion) {
	this->orientacion = orientacion;
}

int Character::getContactosActuales() {
	return this->contactosActuales;
}

b2Vec2 Character::getVelocity() {
	return this->body->GetLinearVelocity();
}

void Character::decreaseJumpCooldown() {
	if (this->jumpCooldown > 0)
		this->jumpCooldown -= 1;
}

void Character::decreaseShootCooldown() {
	if (this->shootCooldown > 0)
		this->shootCooldown -= 1;
}

int Character::getJumpCooldown() {
	return (this->jumpCooldown);
}

int Character::getShootCooldown() {
	return (this->jumpCooldown);
}

void Character::updateLeftContact(int numero) {
	this->contactosIzquierda = numero;
}

void Character::updateRightContact(int numero) {
	this->contactosDerecha = numero;
}

void cambiarEstadoAlAterrizar(Character* character) {
	/*
	 if (character->state != &Character::dying && character->state != &Character::rolling) {
	 if (character->movimientoDerecha == true || character->movimientoIzquierda == true)
	 character->state = &Character::walking;
	 else
	 character->state = &Character::standby;
	 }
	 */
}

void Character::empiezoContacto(b2Fixture* fixture) {

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

void Character::beginContact(Figura* figura, b2Contact* contact) {
	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	if (figuraA == this && figuraB->esEstatico())
		this->empiezoContacto(contact->GetFixtureA());

	if (figuraB == this && figuraA->esEstatico())
		this->empiezoContacto(contact->GetFixtureB());

}

void Character::terminoContacto(b2Fixture* fixture) {

	if (paredDerecha == fixture)
		contactosDerecha--;

	if (paredIzquierda == fixture)
		contactosIzquierda--;

	if (piso == fixture) {

		contactosActuales--;

		if (contactosActuales == 0 && state->getCode() != JUMPING) {
			noAtravezarPlataformas();

		}

	}
}

void Character::atravezarPlataformas() {
	cambiarFilterIndex(FIGURA_FILTER_INDEX);
}

void Character::cambiarFilterIndex(int16 groupIndex) {
	b2Filter filter;
	filter.groupIndex = groupIndex;

	//Recorremos todos los fixtures del objeto y cambiamos el filter
	for (b2Fixture* fix = this->body->GetFixtureList(); fix; fix = fix->GetNext()) {
		fix->SetFilterData(filter);
	}

}

Character::~Character() {
	return;
}

float Character::getX() {
	return (this->body->GetPosition().x);
}

float Character::getY() {
	return (this->body->GetPosition().y);
}

float Character::getAncho() {
	return ancho;
}
float Character::getAlto() {
	return alto;
}

char Character::getId() {
	return '0';
}

void Character::jump() {
	return;
}
void Character::handleInput(SDL_Keycode input, Uint32 input_type) {
	return;
}

void Character::sacarVida() {
	return;
}

void Character::kick() {
	return;
}

void Character::noAtravezarPlataformas() {
	return;
}
;

void Character::detectarEstado() {

	if (state == &Character::dying)
		return;

	if (state == &Character::rolling)
		return;

	//Esta disparando?
	if (movimientoDisparar) {
		state = &Character::shooting;
		return;
	}

	//Esta cayendo?
	if (getVelocity().y <= 0.0f && getContactosActuales() == 0) {
		state = &Character::falling;
		return;
	}

	//Esta quieto?
	if (getVelocity().y == 0.0f && getVelocity().x == 0.0f) {
		state = &Character::standby;
		return;
	}

	//Esta saltando?
	if (getVelocity().y > 0.0f && getContactosActuales() == 0) {
		state = &Character::jumping;
		return;
	}

	//Esta caminando?
	if (movimientoDerecha || movimientoIzquierda) {
		state = &Character::walking;
		return;
	}
}

void Character::controlarEstado() {

	detectarEstado();

	decreaseJumpCooldown();
	decreaseShootCooldown();
	move();
	realizarDisparo();

}
