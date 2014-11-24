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

void Character::teletransportar2() {
	if (teletransportar) {
		body->SetTransform(portal->getDestination(), 0);
		teletransportar = false;
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

bool Character::estaEnAire() {

	//Buscamos si el piso esta haciendo contacto con algo
	for (b2ContactEdge *ce = this->body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->GetFixtureA() == piso || c->GetFixtureB() == piso)
			return false;
	}

	return true;
}

void Character::disparar() {
	this->movimientoDisparar = true;
}

void Character::dejarDisparar() {
	this->movimientoDisparar = false;
}

void Character::shoot() {

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

bool Character::getDisparando() {
	return movimientoDisparar;
}

char Character::getOrientacion() {
	return this->orientacion;
}

void Character::setOrientacion(char orientacion) {
	this->orientacion = orientacion;
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

void Character::decreaseEspera(){
	if (this->espera >0)
		this->espera -=1;
}

int Character::getJumpCooldown() {
	return (this->jumpCooldown);
}

int Character::getShootCooldown() {
	return (this->shootCooldown);
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

void Character::detectarEstado() {

	if (getVelocity().y < 0.0f)
		this->noAtravezarPlataformas();

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
	if (getVelocity().y < 0.0f && estaEnAire()) {
		state = &Character::falling;
		return;
	}

	//Esta quieto?
	if ((getVelocity().y <= 0.005f && getVelocity().y >= -0.005f)
	 && (getVelocity().x <= 0.005f && getVelocity().x >= -0.005f)) {
		state = &Character::standby;
		return;
	}

	//Esta saltando?
	if (getVelocity().y > 0.0f && estaEnAire()) {
		state = &Character::jumping;
		atravezarPlataformas();
		return;
	}

	//Esta moviendose?
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
	shoot();
	//teletransportar2();

}
