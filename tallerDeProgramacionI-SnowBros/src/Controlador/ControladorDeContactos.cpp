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
	b2Fixture* fixture = contact->GetFixtureA();
	b2Fixture* fixtureAux = contact->GetFixtureB();

	switch ((intptr_t)fixture->GetUserData()){

		case ID_RIGHT_WALL_SENSOR:{
			if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
				(*contactosRightSensor)++;
				personaje->updateRightContact((*contactosRightSensor));
			}
			break;
		}

		case ID_LEFT_WALL_SENSOR:{
			if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
				 (*contactosLeftSensor)++;
				 personaje->updateLeftContact((*contactosLeftSensor));
			}
			break;
		}

		case ID_FOOT_SENSOR:{
			if((*contactosSensor) == 0){
				  cambiarEstadoAlAterrizar(personaje);
			 }
			 (*contactosSensor)++;
			 break;
		}
	}

	fixture = contact->GetFixtureB();
	switch ((intptr_t)fixture->GetUserData()){



		case ID_FOOT_SENSOR:{
			if((*contactosSensor) == 0){
				  cambiarEstadoAlAterrizar(personaje);
			 }
			 (*contactosSensor)++;
			 break;
		}
	}
}


void Contacto::EndContact(b2Contact* contact) {
	b2Fixture* fixture = contact->GetFixtureA();
	b2Fixture* fixtureAux = contact->GetFixtureB();

    switch ((intptr_t)fixture->GetUserData()){

		case ID_RIGHT_WALL_SENSOR:{
			if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
				 (*contactosRightSensor)--;
				 personaje->updateRightContact((*contactosRightSensor));
			}
			break;
		}

		case ID_LEFT_WALL_SENSOR:{
			if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
				 (*contactosLeftSensor)--;
				 personaje->updateLeftContact((*contactosLeftSensor));
			}
			break;
		}

		case ID_FOOT_SENSOR:{
			(*contactosSensor)--;
			if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
				personaje->state = &Personaje::falling;
			break;
		}
	}

	fixture = contact->GetFixtureB();
    switch ((intptr_t)fixture->GetUserData()){



		case ID_FOOT_SENSOR:{
			(*contactosSensor)--;
			if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
				personaje->state = &Personaje::falling;
			break;
		}
	}
}
