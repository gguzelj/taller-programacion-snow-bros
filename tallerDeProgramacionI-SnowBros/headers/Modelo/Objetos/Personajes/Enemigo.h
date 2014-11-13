#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Character.h"

#define MITAD_ANCHO_ENEMIGO 0.85f
#define MITAD_ALTO_ENEMIGO 0.85f
#define IZQUIERDA 'l'
#define DERECHA 'r'

#define NIVEL_CONGELAMIENTO_MAX 7

class Escenario;

class Enemigo: public Character {
private:

	Escenario *escenario_;
	int nivelDeCongelamiento;
	int puntos;
	time_t tiempoDeImpactoDeLaUltimaBola;

	void morirDelay();

public:

	bool esAtravezable;
	bool estaVivo;
	int cantidadDeRebotesParaDestruccion = 0;

	b2Fixture* paredIzquierda;
	b2Fixture* paredDerecha;

	Enemigo(JsonParser *parser, int index, Escenario* escenario);
	~Enemigo();
	void disparar();

	void empujar();

	void morir();

	void handleInput(SDL_Keycode input, Uint32 input_type);

	void beginContact(Figura*,b2Contact*);

	void beginContactBolaNieve(BolaNieve*,b2Contact*);

	void beginContactBolaEnemigo(BolaEnemigo*,b2Contact*);

	void congelar();

	void movimientoBola();

	bool estaCongelado();

	void jump();

	int getNivelDeCongelamiento(){
		return nivelDeCongelamiento;
	}

	b2Body* getb2Body() {
		return this->body;
	}

	int getPuntos(){
		return puntos;
	}

	void hacerAtravezable();
	void hacerNoAtravezable();
};

#endif /* ENEMIGO_H_ */
