#ifndef PROYECTIL_H_
#define PROYECTIL_H_

#include <unistd.h>
#include <math.h>
#include <Box2D/Box2D.h>
#include <Box2D/Collision/b2Collision.h>
#include "../Figura.h"

class Proyectil : public Figura {

protected:
	float angulo;
	float masa;
	bool estatico;
	b2World* world;
	b2Body* body;

public:
	float x;
	float y;

	Proyectil();
	virtual ~Proyectil();

	b2Vec2 GetCenter();
	float getRadio();
	float getAngulo();
	b2Body* getb2Body();
};

#endif /* PROYECTIL_H_ */
