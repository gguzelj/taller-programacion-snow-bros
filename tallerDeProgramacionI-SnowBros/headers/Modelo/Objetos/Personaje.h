#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cstdint>
#include "../Objetos/Figuras/Figura.h"

#define DE_PIES 1


class Contacto : public b2ContactListener{
	private:
		int contactosSensor;
	public:
		void updateContacto(int contactosDelSensor){
			this->contactosSensor = contactosDelSensor;
		}

	//Voy a ver si mis pies estan en contacto con algo. Sirve para saber si puedo saltar
    void BeginContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if ( (intptr_t)fixtureUserData == DE_PIES )
            this->contactosSensor++;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ( (intptr_t)fixtureUserData == DE_PIES )
        	this->contactosSensor++;
    }
    //Aca veo cuando sacan los pies el contacto con algo. Sirve para saber si puedo saltar
    void EndContact(b2Contact* contact) {
        void* fixtureUserData = contact->GetFixtureA()->GetUserData();
        if ( (intptr_t)fixtureUserData == DE_PIES )
        	this->contactosSensor--;
        fixtureUserData = contact->GetFixtureB()->GetUserData();
        if ( (intptr_t)fixtureUserData == DE_PIES )
        	this->contactosSensor--;
    }
};


class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;                         //Viene por parametro.
		b2Body* body;
		Contacto contactos;
		int contactosSensor;					//Una pequeña global para saber cuando se toca al sensor del personaje
		int velocidadLimite, aceleracion, frenado;
		int stickiness;                 		//Para mostrar un poco de inercia
		int duracionStickiness;
		bool estaSaltando;

	public:
		Personaje(JsonParser *parser, b2World* world);
		~Personaje();   //Destructor del personaje

		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		void moveLeft();
		void moveRight();
		void jump();
		void stop();

};


#endif /* PERSONAJE_H_ */
