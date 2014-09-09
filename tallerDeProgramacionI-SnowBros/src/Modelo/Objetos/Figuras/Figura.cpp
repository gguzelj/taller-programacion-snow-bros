#include "../../../../headers/Modelo/Objetos/Figuras/Figura.h"

b2Fixture* Figura::GetFixtureList(){
	return body->GetFixtureList();
}

b2Vec2 Figura::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}

std::string Figura::getColor(){
	return color;
}

