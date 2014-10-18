#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cstdint>
#include "../Objetos/Figuras/Figura.h"
#include "../../Controlador/ControladorDeContactos.h"
#include "../Estados/Estados.h"

#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 0.85
#define IZQUIERDA 'l'
#define DERECHA 'r'

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;                         //Viene por parametro.
		b2Body* body;
		Contacto contactos;
		int cantidadDeContactosActuales;
		int cantidadDeContactosIzquierda;
		int cantidadDeContactosDerecha;
		int aceleracion;
		char orientacion;
		int jumpCooldown;

	public:
		static JumpingState jumping;
		static FallingState falling;
		static WalkingState walking;
		static StandByState standby;
		PersonajeState* state;
		char id[20];

		Personaje(float x, float y,char id[], b2World* world);
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
		void decreaseJumpCooldown();
		int getJumpCooldown();
		void updateLeftContact(int);
		void updateRightContact(int);
		void printContactosLR();
};


#endif /* PERSONAJE_H_ */
