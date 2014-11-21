#include "../../../../headers/Modelo/Objetos/Bonus/BonusVidaExtra.h"

BonusVidaExtra::BonusVidaExtra(float posicionX, float posicionY, b2World* world) :
Bonus(posicionX, posicionY, world){
	this->type = ID_BONUS_VIDA_EXTRA;
}

BonusVidaExtra::~BonusVidaExtra(){
}

void BonusVidaExtra::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBonusVidaExtra(this, contact);
}

char BonusVidaExtra::getId(){
	return BONUS_VIDA_CODE;
}
