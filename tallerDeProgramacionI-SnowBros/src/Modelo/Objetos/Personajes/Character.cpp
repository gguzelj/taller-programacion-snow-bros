#include "../../../../headers/Modelo/Objetos/Personajes/Character.h"

void Character::moveLeft() {
	if (this->contactosIzquierda == 0) {
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = -aceleracion;
		this->body->SetLinearVelocity(velocidadActual);
	};
}

void Character::moveRight() {
	if (this->contactosDerecha == 0) {
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity(velocidadActual);
	};
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

int Character::getCantidadDeContactosActuales() {
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
	if(character->state != &Character::dying && character->state != &Character::rolling){
		if (character->movimientoLateralDerecha == true || character->movimientoLateralIzquierda == true)
			character->state = &Character::walking;
		else
			character->state = &Character::standby;
	}
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

void Character::beginContact(Figura* figura, b2Contact* contact){
	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	if(figuraA == this && figuraB->esEstatico())
		this->empiezoContacto(contact->GetFixtureA());

	if(figuraB == this && figuraA->esEstatico())
		this->empiezoContacto(contact->GetFixtureB());

}

void Character::terminoContacto(b2Fixture* fixture) {

		if (paredDerecha == fixture)
			contactosDerecha--;

		if (paredIzquierda == fixture)
			contactosIzquierda--;

		if (piso == fixture) {

			contactosActuales--;

			if (contactosActuales == 0 && state->getCode() != JUMPING){
				noAtravezarPlataformas();
				if(state != &Character::dying && state != &Character::rolling)
					state = &Character::falling;
			}

		}
}

void Character::atravezarPlataformas(){
	cambiarFilterIndex(FIGURA_FILTER_INDEX);
}

void Character::cambiarFilterIndex(int16 groupIndex){
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

float Character::getX(){
	return (this->body->GetPosition().x);
}

float Character::getY(){
	return (this->body->GetPosition().y);
}

float Character::getAncho() {
	return ancho;
}
float Character::getAlto() {
	return alto;
}

char Character::getId(){
	return '0';
}

void Character::jump(){
	return;
}
void Character::handleInput(SDL_Keycode input, Uint32 input_type) {
	return;
}

void Character::disparar() {
	return;
}

void Character::sacarVida() {
	return;
}

void Character::kick(){
	return;
}

void Character::noAtravezarPlataformas(){
	return;
};

void Character::controlarEstado(){

	if (this->state != &Character::dying) {
		//Chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
		this->decreaseJumpCooldown();
		//Chequeo si puedo disparar.
		this->decreaseShootCooldown();

		//Esta implementado aca para que cambie cuando tiene que hacerlo
		if (this->getVelocity().y <= 0.0f && this->getCantidadDeContactosActuales() == 0) {

			if (this->state != &Character::shooting && this->state != &Character::rolling)
				this->state = &Character::falling;
			this->noAtravezarPlataformas();

		} else if (this->getVelocity().y <= 0.0f && this->state == &Character::jumping) {

			this->state = &Character::standby;

		}

		if (this->movimientoLateralDerecha || this->movimientoLateralIzquierda)
			Character::walking.caminar(*this);
	}
}
