#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Character.h"

#define MITAD_ANCHO_ENEMIGO 0.70f
#define MITAD_ALTO_ENEMIGO 0.70
#define IZQUIERDA 'l'
#define DERECHA 'r'

#define NIVEL_CONGELAMIENTO_MAX 7

class Enemigo: public Character {
private:

	int nivelDeCongelamiento;
	int puntos;
	time_t tiempoDeImpactoDeLaUltimaBola;

public:

	bool esAtravezable;
	bool estaVivo;
	bool enMovimientoBola;
	int cantidadDeRebotesParaDestruccion = 0;

	b2Fixture* paredIzquierda;
	b2Fixture* paredDerecha;

	Enemigo(float x, float y, b2World* world);
	~Enemigo();
	void disparar();

	void empujar();

	void morir();

	void handleInput(SDL_Keycode input, Uint32 input_type);

	void reaccionarCon(Figura* figura);

	void reaccionarConBolaNieve(BolaNieve* bola);

	void reaccionarConEnemigo(Enemigo* enemigo);
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
