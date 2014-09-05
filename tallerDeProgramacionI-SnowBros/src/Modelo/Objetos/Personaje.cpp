#include "../../../headers/Modelo/Objetos/Personaje.h"




void Personaje::Moverse(int numeroIngresado){
	//Movimiento con aceleracion, podriamos hacer que si deja de tocar una tecla se empiece a frenar bruscamente
	float apliqueDeEmpuje;
	b2Vec2 velocidadActual = this->cuerpoDinamicoPersonaje->GetLinearVelocity(); //va a servir para cambiarla
	switch (numeroIngresado)
	{
	case -1: //movimiento a izquierda
		if (velocidadActual.x > -(this->velocidadLimite))
		apliqueDeEmpuje = -(this->aceleracion);
	break;

	case 1:	//movimiento a derecha
		if (velocidadActual.x < this->velocidadLimite)
			apliqueDeEmpuje = this->aceleracion;
		break;

	case 0: //no se apreto boton
		apliqueDeEmpuje = velocidadActual.x * (this->frenado);
		break;
	}
	this->cuerpoDinamicoPersonaje->ApplyForce(b2Vec2(apliqueDeEmpuje,0), this->cuerpoDinamicoPersonaje->GetWorldCenter(),true); //tuve que agregar el true porque cambiaron el applyforce, no se que pasa si dejo false
}

void Personaje::Saltar(){

}
