#include "../../../../headers/Modelo/Objetos/Bonus/BonusMoverRapido.h"

void BonusMoverRapido::desactivar(){
	this->activado = false;
}

bool BonusMoverRapido::activo(){
	return this->activado;
}
