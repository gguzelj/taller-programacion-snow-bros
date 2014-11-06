#ifndef PROYECTIL_H_
#define PROYECTIL_H_

#include <math.h>
#include <Box2D/Box2D.h>
#include <Box2D/Collision/b2Collision.h>

class Proyectil {

protected:
	float angulo;
	float masa;
	bool estatico;
	b2World* world;
	b2Body* body;

public:
	std::string type = "proyectil";
	float x;
	float y;

	Proyectil();
	virtual ~Proyectil();

	b2Vec2 GetCenter();
	float getRadio();
	float getAngulo();
};

#endif /* PROYECTIL_H_ */
