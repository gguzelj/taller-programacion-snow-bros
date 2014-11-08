#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "Character.h"
#include "Enemigo.h"

#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 0.85

#define DESCONECTADO -1
#define ESPERANDO 0
#define CONECTADO 1

typedef char conn_id[20];

class Escenario;

class Personaje : public Character {
	private:

		Escenario *escenario_;

		char connectionState;
		int points;

	public:
		conn_id id;

		Personaje(float x, float y,char id[], Escenario* escenario);
		~Personaje();

		b2Vec2 Posicion();
		b2Fixture* GetFixtureList();
		b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

		int getPoints(){
			return points;
		}
		int getLives(){
			return lives;
		}

		void sacarVida(){
			lives--;
		}

		char getConnectionState(){
			return connectionState;
		}
		void setConnectionState(char state){
			connectionState = state;
		}
		void handleInput(SDL_Keycode input,Uint32 input_type);

		void disparar();

		void reaccionarConEnemigo(Enemigo* enemigo);

};


#endif /* PERSONAJE_H_ */
