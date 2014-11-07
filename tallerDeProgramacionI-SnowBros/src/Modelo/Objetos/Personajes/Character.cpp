#include "../../../../headers/Modelo/Objetos/Personajes/Character.h"

void Character::moveLeft(){
	if(this->cantidadDeContactosIzquierda == 0){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = -aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
	};
}

void Character::moveRight(){
	if (this->cantidadDeContactosDerecha == 0){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = aceleracion;
		this->body->SetLinearVelocity( velocidadActual );
	};
}

void Character::jump(){
	if (this->jumpCooldown <= 0){
		 this->jumpCooldown = 18;
			b2Vec2 velocidadActual = this->body->GetLinearVelocity();
			velocidadActual.y = 18;
			this->body->SetLinearVelocity( velocidadActual );
	};
}

void Character::stop(){
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.x = 0;
		if(state == &Character::walking)
			velocidadActual.y = 0;
		this->body->SetLinearVelocity( velocidadActual );
}

b2Fixture* Character::GetFixtureList(){
	return this->body->GetFixtureList();
}

b2Vec2 Character::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}

char Character::getOrientacion(){
	return this->orientacion;
}

void Character::setOrientacion(char orientacion){
	this->orientacion = orientacion;
}

int Character::getCantidadDeContactosActuales(){
	return this->cantidadDeContactosActuales;
}

b2Vec2 Character::getVelocity(){
	return this->body->GetLinearVelocity();
}

float Character::getX(){
	return (this->body->GetPosition().x);
}

float Character::getY(){
	return (this->body->GetPosition().y);
}

void Character::decreaseJumpCooldown(){
	if (this->jumpCooldown > 0)
	this->jumpCooldown -= 1;
}

int Character::getJumpCooldown(){
	return (this->jumpCooldown);
}

void Character::updateLeftContact(int numero){
	this->cantidadDeContactosIzquierda = numero;
}

void Character::updateRightContact(int numero){
	this->cantidadDeContactosDerecha = numero;
}
