#ifndef PROYECTIL_H_
#define PROYECTIL_H_

#include <unistd.h>
#include <math.h>
#include <Box2D/Box2D.h>
#include <Box2D/Collision/b2Collision.h>
#include "../Figura.h"
#include "../Figuras/Rectangulo.h"
#include "../Figuras/Portal.h"

class Proyectil : public Figura {

protected:
	float masa;
	bool estatico;
	b2World* world;

public:
	float x;
	float y;
	float angulo;

	Proyectil();
	virtual ~Proyectil();

	b2Vec2 GetCenter();
	float getRadio();
	float getAngulo();
	b2Body* getb2Body();
};

#endif /* PROYECTIL_H_ */
