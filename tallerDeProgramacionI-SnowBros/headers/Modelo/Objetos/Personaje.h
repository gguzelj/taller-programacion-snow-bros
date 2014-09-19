#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cstdint>
#include "../Objetos/Figuras/Figura.h"
#include "../../Controlador/ControladorDeContactos.h"

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;                         //Viene por parametro.
		b2Body* body;
		Contacto contactos;
		int cantidadDeContactosActuales;
		int aceleracion;
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
