#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cstdint>
#include "../Objetos/Figuras/Figura.h"
#include "../../Controlador/ControladorDeContactos.h"
#include "../Estados/Estados.h"

#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 1

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;                         //Viene por parametro.
		b2Body* body;
		Contacto contactos;
		int cantidadDeContactosActuales;
		int aceleracion;
		char orientacion;

	public:
		static JumpingState jumping;
		static FallingState falling;
		static WalkingState walking;
		static StandByState standby;
		PersonajeState* state;

		Personaje(JsonParser *parser, b2World* world);
		~Personaje();   //Destructor del personaje

		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		float getX();
		float getY();
		b2Vec2 getVelocity();
		int getCantidadDeContactosActuales();
		char getOrientacion();
		void setOrientacion(char orientacion);

		void handleInput(SDL_Keycode input,Uint32 input_type);

		void moveLeft();
		void moveRight();
		void jump();
		void stop();

};


#endif /* PERSONAJE_H_ */
