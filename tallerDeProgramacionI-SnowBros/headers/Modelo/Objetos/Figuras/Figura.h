#ifndef FIGURA_H_
#define FIGURA_H_

#include <Box2D/Box2D.h>
#include <list>
#include "../../../Parser/JsonParser.h"

/*
 * Clase abstracta de la cual van a heredar todos los tipos de figuras (Rectangulo, Poligono, Circulo, Parelelogramo)
 * Dejamos la idea de si Personaje deberia heredar de Figura o no. Osea, en realidad es un figura dinamica?
 */
class Figura{
protected:
	float x;
	float y;
	std::string color;
	int angulo;
	float masa;
	bool estatico;
	b2World* world;
	b2Body* body;
public:
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
