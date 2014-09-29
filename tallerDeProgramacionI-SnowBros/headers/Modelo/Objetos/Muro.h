#ifndef PARED_H_
#define PARED_H_

#define ALTO_CERO 0

#include <Box2D/Box2D.h>
#include <list>
#include <math.h>
#include "../../Parser/JsonParser.h"

class Muro {
private:
	float ancho;
	float x;
	float y;
	float angulo;
	bool estatico;
	b2World* world;
	b2Body* body;
public:
	/*
	 * Para determinar si es el piso o una pared lateral, me fijo en la rotacion. Si es 0, es el piso. Si es 90, es una pared.
	 */
	Muro(float ancho_esc, float alto_esc, int rot, b2World* world, float pos_x);
	~Muro();
};

#endif /* PARED_H_ */
