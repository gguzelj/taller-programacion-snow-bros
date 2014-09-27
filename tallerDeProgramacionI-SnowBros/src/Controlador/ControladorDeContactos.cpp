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
        switch ((intptr_t)fixtureUserData){

			case ID_RIGHT_WALL_SENSOR:{
				 (*contactosRightSensor)++;
				 personaje->updateRightContact((*contactosRightSensor));
			   }


			case ID_LEFT_WALL_SENSOR:{
				 (*contactosLeftSensor)++;
				 personaje->updateLeftContact((*contactosLeftSensor));
			}

			case ID_FOOT_SENSOR:{
				if((*contactosSensor) == 0){
					  cambiarEstadoAlAterrizar(personaje);
				 }
				 (*contactosSensor)++;
			   }
			}


			fixtureUserData = contact->GetFixtureB()->GetUserData();
	        switch ((intptr_t)fixtureUserData){

				case ID_RIGHT_WALL_SENSOR:{
					 (*contactosRightSensor)++;
					 personaje->updateRightContact((*contactosRightSensor));
				   }


				case ID_LEFT_WALL_SENSOR:{
					 (*contactosLeftSensor)++;
					 personaje->updateLeftContact((*contactosLeftSensor));
				}

				case ID_FOOT_SENSOR:{
					if((*contactosSensor) == 0){
						  cambiarEstadoAlAterrizar(personaje);
					 }
					 (*contactosSensor)++;
				   }
				}
}


void Contacto::EndContact(b2Contact* contact) {
	void* fixtureUserData = contact->GetFixtureA()->GetUserData();
    switch ((intptr_t)fixtureUserData){

		case ID_RIGHT_WALL_SENSOR:{
			 (*contactosRightSensor)--;
			 personaje->updateRightContact((*contactosRightSensor));
		   }


		case ID_LEFT_WALL_SENSOR:{
			 (*contactosLeftSensor)--;
			 personaje->updateLeftContact((*contactosLeftSensor));
		}

		case ID_FOOT_SENSOR:{
			(*contactosSensor)--;
			if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
				personaje->state = &Personaje::falling;
		   }
		}

	fixtureUserData = contact->GetFixtureB()->GetUserData();
    switch ((intptr_t)fixtureUserData){

		case ID_RIGHT_WALL_SENSOR:{
			 (*contactosRightSensor)--;
			 personaje->updateRightContact((*contactosRightSensor));
		   }


		case ID_LEFT_WALL_SENSOR:{
			 (*contactosLeftSensor)--;
			 personaje->updateLeftContact((*contactosLeftSensor));
		}

		case ID_FOOT_SENSOR:{
			(*contactosSensor)--;
			if(*contactosSensor == 0 && personaje->state->getCode() != JUMPING)
				personaje->state = &Personaje::falling;
		   }
		}
}
