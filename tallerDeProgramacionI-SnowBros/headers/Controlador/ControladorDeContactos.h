#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

#include "Box2D/Box2D.h"

class Personaje;

const int ID_FOOT_SENSOR = 3;

// http://www.iforce2d.net/b2dtut/collision-callbacks
// Se usan variables booleanas ya que solo nos interesa si toca el suelo
class Contacto : public b2ContactListener{
private:
	int* contactosSensor;

	Personaje* personaje;

public:

	void setPersonaje(Personaje* personaje){
		this->personaje = personaje;
	}

	void updateContacto(int* contactosDelSensor){
		this->contactosSensor = contactosDelSensor;
	}

	//Voy a ver si el personaje esta en contacto con algo, esta sobre una plataforma o en el aire?
	//Se esta tomando al cuerpo entero, no a los pies
	void BeginContact(b2Contact* contact);

	//Veo si el personaje dejo de estar en contacto con algo.
	void EndContact(b2Contact* contact);
};


#endif /* CONTROLADORDECONTACTOS_H_ */
