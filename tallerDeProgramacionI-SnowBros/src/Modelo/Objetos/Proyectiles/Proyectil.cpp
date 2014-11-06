#include "../../../../headers/Modelo/Objetos/Proyectiles/Proyectil.h"

Proyectil::Proyectil() {
	// TODO Auto-generated constructor stub

}

Proyectil::~Proyectil() {
	// TODO Auto-generated destructor stub
}

b2Vec2 Proyectil::GetCenter(){
	return body->GetWorldCenter();
}

float Proyectil::getAngulo() {
	return this->body->GetAngle();
}
