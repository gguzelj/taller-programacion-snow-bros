#include "../../../../headers/Modelo/Objetos/Bonus/BonusMoverRapido.h"

BonusMoverRapido::BonusMoverRapido(float posicionX, float posicionY, b2World* world) :
Bonus(posicionX, posicionY, world){
	this->type = ID_BONUS_MOVER_RAPIDO;
}

BonusMoverRapido::~BonusMoverRapido(){
}

void BonusMoverRapido::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBonusMoverRapido(this, contact);
}

char BonusMoverRapido::getId(){
	return RECTANGULO_CODE;
}
