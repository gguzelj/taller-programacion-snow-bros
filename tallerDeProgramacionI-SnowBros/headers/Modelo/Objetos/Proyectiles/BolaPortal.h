#ifndef SRC_MODELO_OBJETOS_PROYECTILES_BOLAPORTAL_H_
#define SRC_MODELO_OBJETOS_PROYECTILES_BOLAPORTAL_H_

#define RADIO_BOLA_PORTAL 0.35
#define MASA_BOLA_PORTAL 1

#include "Proyectil.h"

class BolaPortal: public Proyectil {

private:
	float radio;

public:
	bool crearPortal;
	Personaje* personaje;

	BolaPortal(float x, float y, int potencia, b2World* world, Personaje*);
	virtual ~BolaPortal();

	void beginContactRectangulo(Rectangulo* rec,b2Contact* contact);

	void setVelocidad(b2Vec2 velocidad);
	float getRadio();

	float getAlto();
	float getAncho();
	char getId();

	Portal* crearNuevoPortal();

	b2Body* getb2Body(){
		return body;
	}
};

#endif /* SRC_MODELO_OBJETOS_PROYECTILES_BOLAPORTAL_H_ */
