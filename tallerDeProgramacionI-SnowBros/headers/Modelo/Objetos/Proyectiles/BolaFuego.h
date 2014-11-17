#ifndef SRC_MODELO_OBJETOS_PROYECTILES_BOLAFUEGO_H_
#define SRC_MODELO_OBJETOS_PROYECTILES_BOLAFUEGO_H_

#define RADIO_BOLA_FUEGO 0.35
#define MASA_BOLA_FUEGO 1

#include "Proyectil.h"

class BolaFuego: public Proyectil {

private:
	b2Vec2 velocidad;
	float radio;

public:

	bool destruir;
	int potencia;

	BolaFuego(float x, float y, int potencia, b2World* world);
	virtual ~BolaFuego();

	void actualizar();

	void morir();

	void setVelocidad(b2Vec2 velocidad);

	b2Vec2 getVelocidad();

	float getRadio();

	float getAlto();

	float getAncho();

	char getId();

	void beginContact(Figura*, b2Contact*);

	b2Body* getb2Body();
};

#endif /* SRC_MODELO_OBJETOS_PROYECTILES_BOLAFUEGO_H_ */
