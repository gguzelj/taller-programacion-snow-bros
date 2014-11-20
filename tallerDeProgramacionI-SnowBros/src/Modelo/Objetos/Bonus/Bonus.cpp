#include "../../../../headers/Modelo/Objetos/Bonus/Bonus.h"



void Bonus::desactivar(){
	this->activado = false;
}

bool Bonus::activo(){
	return this->activado;
}
