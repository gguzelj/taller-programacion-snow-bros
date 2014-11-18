#include "../../../../headers/Modelo/Objetos/Bonus/BonusVidaExtra.h"

void BonusVidaExtra::desactivar(){
	this->activado = false;
}

bool BonusVidaExtra::activo(){
	return this->activado;
}
