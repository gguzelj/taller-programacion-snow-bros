#ifndef BOLAENEMIGO_H_
#define BOLAENEMIGO_H_

#define RADIO_BOLA_NIEVE_ENEMIGO 1
#define MASA_BOLA_NIEVE_ENEMIGO 100

#include <unistd.h>
#include "Proyectil.h"

class BolaEnemigo: public Proyectil {

private:
	float radio;

public:

	bool destruir;

	BolaEnemigo(float x, float y,b2World* world);
	virtual ~BolaEnemigo();

	void setVelocidad(b2Vec2 velocidad);

	float getRadio();

	float getAlto();

	float getAncho();

	char getId();

	void morir();

	b2Body* getb2Body(){
		return body;
	}

	void reaccionarCon(Figura* fig);

};

#endif /* BOLAENEMIGO_H_ */
