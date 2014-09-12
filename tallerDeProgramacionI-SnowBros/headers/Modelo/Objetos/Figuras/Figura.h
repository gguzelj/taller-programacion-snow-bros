#ifndef FIGURA_H_
#define FIGURA_H_

#include <Box2D/Box2D.h>
#include <list>
#include <math.h>
#include "../../../Parser/JsonParser.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

/*
 * Clase abstracta de la cual van a heredar todos los tipos de figuras (Rectangulo, Poligono, Circulo, Parelelogramo)
 * Dejamos la idea de si Personaje deberia heredar de Figura o no. Osea, en realidad es un figura dinamica?
 */
class Figura{
protected:
	std::string color;
	float angulo;
	float masa;
	bool estatico;
	b2World* world;
	b2Body* body;
public:
	float x;
	float y;
	virtual ~Figura(){
		//this->world->DestroyBody(this->body);
	}
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
	std::string getColor();
	float getX();
	float getY();
	int getAngulo();
	float getMasa();
	bool esEstatico();
	b2Body* getb2Body();
};

#endif /* FIGURA_H_ */
