#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

#include "../Modelo/Objetos/Personaje.h"

// http://www.iforce2d.net/b2dtut/collision-callbacks
// Se usan variables booleanas ya que solo nos interesa si toca el suelo
class Contacto : public b2ContactListener{
	//Voy a ver si el personaje esta en contacto con algo, esta sobre una plataforma o en el aire?
	//Se esta tomando al cuerpo entero, no a los pies
    void BeginContact(b2Contact* contact) {

    	//Check if fixture A of the collision is the guy.
        void* bodyUserData = contact->GetFixtureA()->GetUserData();
        if ( bodyUserData ){
        	static_cast<Personaje*> bodyUserData->startContact();
        }
        //Check if fixture B of the collision is the guy.
        bodyUserData = contact->GetFixtureB()->GetUserData();
        if ( bodyUserData )
        	static_cast<Personaje*>( bodyUserData )->startContact();
    }

    //Veo si el personaje dejo de estar en contacto con algo.
    void EndContact(b2Contact* contact) {

    	//Check if fixture A of the collision is the guy.
        void* bodyUserData = contact->GetFixtureA()->GetUserData();
        if ( bodyUserData )
        	static_cast<Personaje*>( bodyUserData )->endContact();

        //Check if fixture B of the collision is the guy.
        bodyUserData = contact->GetFixtureB()->GetUserData();
        if ( bodyUserData )
        	static_cast<Personaje*>( bodyUserData )->endContact();
    }
};


#endif /* CONTROLADORDECONTACTOS_H_ */
