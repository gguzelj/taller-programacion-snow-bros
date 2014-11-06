#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <cstdint>
#include "Proyectiles/BolaNieve.h"
#include "Figuras/Figura.h"
#include "../../Controlador/ControladorDeContactos.h"
#include "../Estados/Estados.h"

#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 0.85
#define IZQUIERDA 'l'
#define DERECHA 'r'

#define DESCONECTADO -1
#define ESPERANDO 0
#define CONECTADO 1

typedef char conn_id[20];

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;
		b2Body* body;

		int aceleracion;
		char orientacion;
		char connectionState;
		int jumpCooldown;
		float ancho;
		float alto;
		int points;
		int lives;

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
		static ShootingState shooting;
		PersonajeState* state;
		conn_id id;

		Personaje(float x, float y,char id[], b2World* world);
		~Personaje();

		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		float getX();
		float getY();
		//Y estos por que estan definidos aca?
		float getAncho(){
			return ancho;
		}
		float getAlto(){
			return alto;
		}
		int getPoints(){
			return points;
		}
		int getLives(){
			return lives;
		}
		char getConnectionState(){
			return connectionState;
		}
		void setConnectionState(char state){
			connectionState = state;
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
		void stop();
		void tirarBolaNieve();
		void decreaseJumpCooldown();

		int getJumpCooldown(){
			return (this->jumpCooldown);
		}

		void updateLeftContact(int);
		void updateRightContact(int);
};


#endif /* PERSONAJE_H_ */
