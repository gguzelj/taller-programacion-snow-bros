#ifndef BOLANIEVE_H_
#define BOLANIEVE_H_

#define RADIO_BOLA_NIEVE 0.35
#define MASA_BOLA_NIEVE 1

#include "Proyectil.h"

class BolaNieve: public Proyectil {

private:
	float radio;

public:
	int potencia;

	BolaNieve(float x, float y, int potencia, b2World* world);
	virtual ~BolaNieve();

	void setVelocidad(b2Vec2 velocidad);
	float getRadio();

	float getAlto();
	float getAncho();
	char getId();

	void beginContact(Figura*,b2Contact*);

};

#endif /* BOLANIEVE_H_ */
