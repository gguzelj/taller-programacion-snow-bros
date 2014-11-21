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
protected:

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

	virtual void mover();

	b2Body* getb2Body();

	bool estaCongelado();

	int getNivelDeCongelamiento();
	int getPuntos();

	void beginContact(Figura*, b2Contact*);
	void beginContactBolaNieve(BolaNieve*, b2Contact*);
	void beginContactBolaEnemigo(BolaEnemigo*, b2Contact*);
	void endContact(Figura*, b2Contact*);
	void congelar();
	void controlarEstado();
	void disparar();
	void empujar();
	void hacerAtravezable();
	void hacerNoAtravezable();
	void handleInput(SDL_Keycode input, Uint32 input_type);
	void jump();
	void morir();
	void movimientoBola();

};

#endif /* ENEMIGO_H_ */
