#ifndef BOLANIEVE_H_
#define BOLANIEVE_H_

#define RADIO_BOLA_NIEVE 0.35
#define MASA_BOLA_NIEVE 1

#include "Proyectil.h"


class BolaNieve: public Proyectil {

private:
	float radio;

public:

	BolaNieve(float x, float y, b2World* world);
	virtual ~BolaNieve();

	void setVelocidad(b2Vec2 velocidad);
	float getRadio();
};

#endif /* BOLANIEVE_H_ */
