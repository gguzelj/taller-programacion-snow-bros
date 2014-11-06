#include "../../headers/Controlador/ControladorDeContactos.h"
#include "../../headers/Modelo/Objetos/Personaje.h"
#include "../../headers/Modelo/Estados/Estados.h"
#include "../../headers/Modelo/Objetos/Enemigo.h"

void cambiarEstadoAlAterrizar(Character* character){
	if(character->movimientoLateralDerecha == true || character->movimientoLateralIzquierda == true)
		character->state = &Character::walking;
	else
		character->state = &Character::standby;
}


void Contacto::actualizarCharacters(std::list<Enemigo*> enemigos, std::list<Personaje*> personajes){
	characters->clear();
	for(auto enemigo = enemigos.begin(); enemigo != enemigos.end(); enemigo++){
		this->characters->push_back((Character*)(*enemigo));
	}
	for(auto personaje = personajes.begin(); personaje != personajes.end(); personaje++){
		this->characters->push_back((Character*)(*personaje));
	}

}




void Contacto::BeginContact(b2Contact* contact) {
	this->actualizarCharacters(*enemigos,*personajes);

	b2Fixture* fixture = contact->GetFixtureA();
	b2Fixture* fixtureAux = contact->GetFixtureB();


	switch ((intptr_t)fixture->GetUserData()){

	case ID_RIGHT_WALL_SENSOR:{
		if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
			for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
				if(((*personaje)->getb2Body()) == (fixture->GetBody())){
					(*personaje)->cantidadDeContactosDerecha++;
				}
			}
		}
		break;
	}

	case ID_LEFT_WALL_SENSOR:{
		if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
			for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
				if((*personaje)->getb2Body() == fixture->GetBody()){
					(*personaje)->cantidadDeContactosIzquierda++;
				}
			}
		}
		break;
	}

	case ID_FOOT_SENSOR:{
		for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
			if((*personaje)->getb2Body() == fixture->GetBody()){
				if((*personaje)->cantidadDeContactosActuales == 0)
					cambiarEstadoAlAterrizar(*personaje);
				(*personaje)->cantidadDeContactosActuales++;
			}
		}
		break;
	}
	}

	fixture = contact->GetFixtureB();
	switch ((intptr_t)fixture->GetUserData()){

	case ID_FOOT_SENSOR:{
		for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
			if((*personaje)->getb2Body() == fixture->GetBody()){
				if((*personaje)->cantidadDeContactosActuales == 0)
					cambiarEstadoAlAterrizar(*personaje);
				(*personaje)->cantidadDeContactosActuales++;
			}
		}
		break;
	}
	}


}


void Contacto::EndContact(b2Contact* contact) {
	this->actualizarCharacters(*enemigos,*personajes);
	b2Fixture* fixture = contact->GetFixtureA();
	b2Fixture* fixtureAux = contact->GetFixtureB();

    switch ((intptr_t)fixture->GetUserData()){

    case ID_RIGHT_WALL_SENSOR:{
    	if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
    		for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
    			if(((*personaje)->getb2Body()) == (fixture->GetBody())){
    				(*personaje)->cantidadDeContactosDerecha--;
    			}
    		}
    	}
    	break;
    }

    case ID_LEFT_WALL_SENSOR:{
    	if(fixture->GetBody()->GetType() == b2_dynamicBody && fixtureAux->GetBody()->GetType() == b2_staticBody){
    		for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
    			if((*personaje)->getb2Body() == fixture->GetBody()){
    				(*personaje)->cantidadDeContactosIzquierda--;
    			}
    		}
    	}
			break;
		}

		case ID_FOOT_SENSOR:{
			for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
				if((*personaje)->getb2Body() == fixture->GetBody()){
					(*personaje)->cantidadDeContactosActuales--;
					if((*personaje)->cantidadDeContactosActuales == 0 && (*personaje)->state->getCode() != JUMPING)
						(*personaje)->state = &Personaje::falling;
				}
			}
			break;
		}
    }

    fixture = contact->GetFixtureB();
    switch ((intptr_t)fixture->GetUserData()){

    case ID_FOOT_SENSOR:{
    	for(auto personaje = characters->begin(); personaje != characters->end(); personaje++){
    		if((*personaje)->getb2Body() == fixture->GetBody()){
    			(*personaje)->cantidadDeContactosActuales--;
    			if((*personaje)->cantidadDeContactosActuales == 0 && (*personaje)->state->getCode() != JUMPING)
    				(*personaje)->state = &Personaje::falling;
    		}
    	}
    	break;
    }
    }

}

