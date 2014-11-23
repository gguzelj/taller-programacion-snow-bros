#include "../../../../headers/Modelo/Objetos/Bonus/BonusBolaPortal.h"

BonusBolaPortal::BonusBolaPortal(float posicionX, float posicionY, b2World* world) :
Bonus(posicionX, posicionY, world){
	this->type = ID_BONUS_VIDA_EXTRA;
}

BonusBolaPortal::~BonusBolaPortal(){
}

void BonusBolaPortal::beginContact(Figura* figura, b2Contact* contact) {
	figura->beginContactBonusBolaPortal(this, contact);
}

char BonusBolaPortal::getId(){
	return BONUS_PORTAL_CODE;
}
