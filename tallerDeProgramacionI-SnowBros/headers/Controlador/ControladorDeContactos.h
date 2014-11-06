#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

#include "Box2D/Box2D.h"
#include <iostream>
#include <list>

class Personaje;
class Enemigo;
class Character;

const int ID_FOOT_SENSOR = 3;
const int ID_LEFT_WALL_SENSOR = 4;
const int ID_RIGHT_WALL_SENSOR = 5;

// http://www.iforce2d.net/b2dtut/collision-callbacks
// Se usan variables booleanas ya que solo nos interesa si toca el suelo
class Contacto : public b2ContactListener{
private:

	int* contactosLeftSensor;
	int* contactosRightSensor;

	std::list<Character*>* characters;
	std::list<Personaje*>* personajes;
	std::list<Enemigo*>* enemigos;

	void actualizarCharacters(std::list<Enemigo*> enemigos, std::list<Personaje*> personajes);

public:
	int* contactosSensor;
	void setPersonaje(std::list<Personaje*>* personajes){
		this->personajes = personajes;
	}
	void setEnemigos(std::list<Enemigo*>* enemigos){
		this->enemigos = enemigos;
	}
	void setCharacters(){
		characters = new std::list<Character*>();
	}

	void updateContacto(int* contactosDelSensor, int* contactosDelSensorL, int* contactosDelSensorR){
		this->contactosSensor = contactosDelSensor;
		this->contactosLeftSensor = contactosDelSensorL;
		this->contactosRightSensor = contactosDelSensorR;
	}

	//Voy a ver si el personaje esta en contacto con algo, esta sobre una plataforma o en el aire?
	//Se esta tomando al cuerpo entero, no a los pies
	void BeginContact(b2Contact* contact);

	//Veo si el personaje dejo de estar en contacto con algo.
	void EndContact(b2Contact* contact);
};


#endif /* CONTROLADORDECONTACTOS_H_ */
