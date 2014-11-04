#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include <cstdint>
#include "../../Controlador/ControladorDeContactos.h"
#include "../Objetos/Figuras/Figura.h"
#include "../Estados/Estados.h"

#define MITAD_ANCHO_ENEMIGO 0.6f
#define MITAD_ALTO_ENEMIGO 0.85
#define IZQUIERDA 'l'
#define DERECHA 'r'

class Enemigo : Figura{
	private:
		b2Vec2 posicion;
		b2World* world;
		b2Body* body;

		int aceleracion;
		char orientacion;
		float ancho;
		float alto;
		int jumpCooldown;

	public:
		bool movimientoLateralDerecha,
			 movimientoLateralIzquierda,
			 debeSaltar;
		int cantidadDeContactosActuales;
		int cantidadDeContactosIzquierda;
		int cantidadDeContactosDerecha;
		static JumpingState jumping;
		static FallingState falling;
		static WalkingState walking;
		static StandByState standby;
		Personaje* state;

		Enemigo(float x, float y, b2World* world);
		~Enemigo();

		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		float getX();
		float getY();

		float getAncho(){
			return ancho;
		}
		float getAlto(){
			return alto;
		}
		b2Body* getb2Body(){
			return this->body;
		}

		b2Vec2 getVelocity();
		int getCantidadDeContactosActuales();

		char getOrientacion(){
			return this->orientacion;
		}

		void setOrientacion(char orientacion);

		void handleInput(SDL_Keycode input,Uint32 input_type);

		void moveLeft();
		void moveRight();
		void jump();
		void decreaseJumpCooldown();

		int getJumpCooldown(){
			return (this->jumpCooldown);
		}

		void updateLeftContact(int);
		void updateRightContact(int);

};

#endif /* ENEMIGO_H_ */
