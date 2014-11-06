#ifndef BOLANIEVE_H_
#define BOLANIEVE_H_

#define RADIO_BOLA_NIEVE 1
#define MASA_BOLA_NIEVE 1

#include "Proyectil.h"


class BolaNieve: public Proyectil {

private:
	float radio;

public:

	BolaNieve(b2World* world);
	virtual ~BolaNieve();

	void setVelocidad(b2Vec2 velocidad);
};

#endif /* BOLANIEVE_H_ */
