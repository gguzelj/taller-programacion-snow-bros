#ifndef BOLAENEMIGO_H_
#define BOLAENEMIGO_H_

#define RADIO_BOLA_NIEVE_ENEMIGO 1
#define MASA_BOLA_NIEVE_ENEMIGO 100

#include <unistd.h>
#include "Proyectil.h"

class BolaEnemigo: public Proyectil {

private:
	b2Vec2 velocidad;
	float radio;

public:

	bool destruir;

	BolaEnemigo(float x, float y,b2World* world);
	virtual ~BolaEnemigo();

	b2Vec2 getPosicion(){
		return (this->body)->GetPosition();
	}

	void setVelocidad(b2Vec2 velocidad);

	b2Vec2 getVelocidad(){
		return this->body->GetLinearVelocity();
	}

	float getRadio();

	float getAlto();

	float getAncho();

	char getId();

	void morir();

	b2Body* getb2Body(){
		return body;
	}

	void actualizar();

	void beginContact(Figura*, b2Contact*);

	void cambiarFilterIndex(int16 groupIndex);

};

#endif /* BOLAENEMIGO_H_ */
