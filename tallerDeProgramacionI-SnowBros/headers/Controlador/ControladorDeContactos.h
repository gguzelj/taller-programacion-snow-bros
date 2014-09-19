#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

const int ID_FOOT_SENSOR = 3;

// http://www.iforce2d.net/b2dtut/collision-callbacks
// Se usan variables booleanas ya que solo nos interesa si toca el suelo
class Contacto : public b2ContactListener{
private:
	int* contactosSensor;
public:
	void updateContacto(int* contactosDelSensor){
		this->contactosSensor = contactosDelSensor;
	}

	//Voy a ver si el personaje esta en contacto con algo, esta sobre una plataforma o en el aire?
	//Se esta tomando al cuerpo entero, no a los pies
	void BeginContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();

        if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR)
           (*contactosSensor)++;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR)
	       	(*contactosSensor)++;
	    }

	//Veo si el personaje dejo de estar en contacto con algo.
	void EndContact(b2Contact* contact) {
		void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR)
			(*contactosSensor)--;
		fixtureUserData = contact->GetFixtureB()->GetUserData();
		if ((intptr_t)fixtureUserData == ID_FOOT_SENSOR)
			(*contactosSensor)--;
    }
};


#endif /* CONTROLADORDECONTACTOS_H_ */
