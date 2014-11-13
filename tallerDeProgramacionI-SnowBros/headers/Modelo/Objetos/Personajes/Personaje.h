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

class Personaje: public Character {
private:

	b2Vec2* posicionInicial;
	Escenario *escenario_;

	char connectionState;

	int points;
	int kickCooldown;

	void entrarEnPeriodoDeInmunidad();
	void hacerInmune();

public:

	b2Body* getb2Body();
	b2Fixture* GetFixtureList();
	b2Joint* joint;
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
	b2Vec2 Posicion();
	BolaEnemigo* arrastradoPor;
	conn_id id;

	bool esta_muerto;
	bool arrastrado;

	Personaje(float x, float y, char id[], Escenario* escenario);
	~Personaje();

	bool getInmune();

	char getConnectionState();
	char getId();

	int getKickCooldown();
	int getLives();
	int getPoints();

	void beginContactBolaEnemigo(BolaEnemigo*, b2Contact*);
	void beginContactEnemigo(Enemigo*, b2Contact*);
	void controlarEstado();
	void decreaseKickCooldown();
	void disparar();
	void empiezoContacto(b2Fixture* fixture);
	void empujar();
	void handleInput(SDL_Keycode input, Uint32 input_type);
	void jump();
	void morir();
	void noAtravezarPlataformas();
	void sacarVida();
	void setConnectionState(char state);
	void terminoContacto(b2Fixture* fixture);
	void volverAPosicionInicial();

};

#endif /* PERSONAJE_H_ */
