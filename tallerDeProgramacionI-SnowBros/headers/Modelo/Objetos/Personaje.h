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


#define DESCONECTADO -1
#define ESPERANDO 0
#define CONECTADO 1

typedef char conn_id[20];

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world;                         //Viene por parametro.
		b2Body* body;
		//Contacto contactos;

		int aceleracion;
		char orientacion;
		char connectionState;
		int jumpCooldown;
		float ancho;
		float alto;

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
		PersonajeState* state;
		conn_id id;

		Personaje(float x, float y,char id[], b2World* world);
		~Personaje();   //Destructor del personaje

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
