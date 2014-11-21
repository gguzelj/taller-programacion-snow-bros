#include "../../../../headers/Modelo/Objetos/Bonus/BonusAumentarPotencia.h"

BonusAumentarPotencia::BonusAumentarPotencia(float posicionX, float posicionY, b2World* world) :
Bonus(posicionX, posicionY, world){
	this->type = ID_BONUS_AUMENTAR_POTENCIA;
}

BonusAumentarPotencia::~BonusAumentarPotencia(){
}

void BonusAumentarPotencia::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBonusAumentarPotencia(this, contact);
}

char BonusAumentarPotencia::getId(){
	return RECTANGULO_CODE;
}
