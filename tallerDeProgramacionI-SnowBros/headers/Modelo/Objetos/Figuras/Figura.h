#ifndef FIGURA_H_
#define FIGURA_H_


#define FIGURA_WARNING_OVERLAP "El siguiente objeto se esta superponiendo con otros ya creados \n"
#include <Box2D/Box2D.h>
#include <Box2D/Collision/b2Collision.h>
#include <list>
#include <math.h>
#include "../../../Parser/JsonParser.h"

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
	std::string type = "figura";
	float x;
	float y;
	virtual ~Figura(){
		//this->world->DestroyBody(this->body);
	}

	bool validarOverlap();
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
	b2Vec2 GetCenter();
	std::string getColor();
	float getX();
	float getY();
	float getAngulo();
	float getMasa();
	bool esEstatico();
	b2Body* getb2Body();
};

#endif /* FIGURA_H_ */
