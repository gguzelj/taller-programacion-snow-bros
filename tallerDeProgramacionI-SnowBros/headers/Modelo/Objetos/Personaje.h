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
		int nombreDeLosContactos;
		int movimientoIzquierda, movimientoDerecha, movimientoNada, velocidadLimite, aceleracion, frenado;
		void Saltar(b2Vec2 VelocidadActual);
		int stickiness;                 		//Para mostrar un poco de inercia
		int duracionStickiness;
		int tiempoEvitaSaltos;
        enum movimientos {
            Mov_Nada,
            Mov_Izquierda,
            Mov_Derecha,
        };
		movimientos movimientoActual;
		movimientos movimientoAnterior; //Va a servir para no interrumpir el movimiento al momento del salto

	public:
		Personaje(JsonParser *parser, b2World* world);
		~Personaje();   //Destructor del personaje
		void Moverse(); //Alguna clase controlador o algo, hasta tal vez un int segun que tipo de movimiento se eligio
		b2Vec2 Posicion();
		void Keyboard(unsigned char key); //Deberia estar aca o en otro lado?
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

};


#endif /* PERSONAJE_H_ */
