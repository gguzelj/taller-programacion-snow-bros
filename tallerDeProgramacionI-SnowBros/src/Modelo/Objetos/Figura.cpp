#include "../../../headers/Modelo/Objetos/Figura.h"

b2Fixture* Figura::GetFixtureList() {
	return body->GetFixtureList();
}

b2Vec2 Figura::GetWorldPoint(const b2Vec2& localPoint) {
	return body->GetWorldPoint(localPoint);
}

b2Vec2 Figura::GetCenter(){
	return body->GetWorldCenter();
}

b2Body* Figura::getBody(){
	return this->body;
}

bool Figura::esEstatico(){
	return estatico;
}

float Figura::getAngulo() {
	return this->body->GetAngle();
}

/**
 * Testeamos si la figura que estamos creando se superpone con algun otro objeto
 * que ya exista en el mundo. En ese caso, no se guarda el mismo
 */
bool Figura::validarOverlap() {

	bool overlap = false;

	//Recorremos todos los objetos del mundo en busca de colisiones
	for (b2Body* currBody = this->world->GetBodyList(); currBody; currBody = currBody->GetNext()) {

		if (currBody == this->body)
			continue;

		overlap = b2TestOverlap(currBody->GetFixtureList()->GetShape(), 0,
								this->body->GetFixtureList()->GetShape(), 0,
								currBody->GetTransform(),
								this->body->GetTransform());
		if (overlap) {
			this->world->DestroyBody(this->body);
			break;
		}
	}

	return overlap;
}
