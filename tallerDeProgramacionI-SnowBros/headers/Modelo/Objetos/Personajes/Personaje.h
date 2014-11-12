#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Character.h"
#include "Enemigo.h"

#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 0.85

#define DESCONECTADO -1
#define ESPERANDO 0
#define CONECTADO 1

#define TIEMPO_INMUNIDAD 5

typedef char conn_id[20];

class Escenario;

class Personaje : public Character {
	private:

		Escenario *escenario_;

		char connectionState;

		int points;

		int kickCooldown;

		b2Vec2* posicionInicial;

		void hacerInmune();

		void entrarEnPeriodoDeInmunidad();

	public:
		conn_id id;

		bool esta_muerto;

		bool arrastrado;

		Enemigo* arrastradoPor;

		b2Joint* joint;

		Personaje(float x, float y,char id[], Escenario* escenario);
		~Personaje();

		void jump();
		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		b2Body* getb2Body(){
			return body;
		}

		int getPoints(){
			return points;
		}

		int getLives(){
			return lives;
		}

		bool getInmune(){
			return inmune;
		}

		int getKickCooldown(){
			return kickCooldown;
		}

		void decreaseKickCooldown();

		void sacarVida(){
			lives--;
		}

		void morir();

		char getConnectionState(){
			return connectionState;
		}
		void setConnectionState(char state){
			connectionState = state;
		}
		void handleInput(SDL_Keycode input,Uint32 input_type);

		void disparar();

		void empujar();

		void reaccionarConEnemigo(Enemigo* enemigo);

		//Metodos para controlar el estado del personaje
		void empiezoContacto(b2Fixture* fixture);
		void terminoContacto(b2Fixture* fixture);
		void volverAPosicionInicial();
		void noAtravezarPlataformas();

};


#endif /* PERSONAJE_H_ */
