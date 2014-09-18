#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

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

        // Ya que solo tenemos un body con user data, si es distinto a null, entonces es el personaje
        if (fixtureUserData )
           (*contactosSensor)++;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
	    if ( fixtureUserData )
	       	(*contactosSensor)++;
	    }

	//Veo si el personaje dejo de estar en contacto con algo.
	void EndContact(b2Contact* contact) {
		void* fixtureUserData = contact->GetFixtureA()->GetUserData();
		if ( fixtureUserData)
			(*contactosSensor)--;
		fixtureUserData = contact->GetFixtureB()->GetUserData();
		if ( fixtureUserData )
			(*contactosSensor)--;
    }
};


#endif /* CONTROLADORDECONTACTOS_H_ */
