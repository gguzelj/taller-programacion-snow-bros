#include "../../headers/Controlador/ControladorDeContactos.h"
#include "../../headers/Modelo/Objetos/Personaje.h"
#include "../../headers/Modelo/Estados/Estados.h"

void cambiarEstadoAlAterrizar(Personaje* personaje){
	if(Personaje::walking.movimientoLateralDerecha == true || Personaje::walking.movimientoLateralIzquierda == true)
		personaje->state = &Personaje::walking;
	else
		personaje->state = &Personaje::standby;
}


void Contacto::BeginContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();


        if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR){
        	if((*contactosSensor) == 0){
        	      cambiarEstadoAlAterrizar(personaje);
        	 }
        	 (*contactosSensor)++;
        }

        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR){
        	if((*contactosSensor) == 0){
        	      cambiarEstadoAlAterrizar(personaje);
        	}
        	(*contactosSensor)++;
        }
}


void Contacto::EndContact(b2Contact* contact) {
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
	if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR){
		(*contactosSensor)--;
		if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
			personaje->state = &Personaje::falling;
	}

	fixtureUserData = contact->GetFixtureB()->GetUserData();
	if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR){
		(*contactosSensor)--;
		if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
			personaje->state = &Personaje::falling;
	}
}
