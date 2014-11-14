#include "../../../../headers/Modelo/Objetos/Proyectiles/Proyectil.h"

b2Vec2 Proyectil::GetCenter(){
	return body->GetWorldCenter();
}

float Proyectil::getAngulo() {
	return this->body->GetAngle();
}

b2Body* Proyectil::getb2Body(){
	return this->body;
}
