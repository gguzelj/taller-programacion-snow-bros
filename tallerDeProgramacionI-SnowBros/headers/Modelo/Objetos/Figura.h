#ifndef FIGURA_H_
#define FIGURA_H_

#define FIGURA_WARNING_OVERLAP "El siguiente objeto se esta superponiendo con otros ya creados \n"
#include <Box2D/Box2D.h>
#include <Box2D/Collision/b2Collision.h>
#include <list>
#include <math.h>
#include "../../Parser/JsonParser.h"

class Enemigo;
class Circulo;
class Muro;
class Paralelogramo;
class Poligono;
class Rectangulo;
class Trapecio;
class Personaje;
class BolaNieve;

/*
 * Clase abstracta de la cual van a heredar todos los tipos de figuras (Rectangulo, Poligono, Circulo, Parelelogramo)
 * Dejamos la idea de si Personaje deberia heredar de Figura o no. Osea, en realidad es un figura dinamica?
 */
class Figura {

protected:
	float angulo;
	float masa;
	bool estatico;
	b2World* world;
	b2Body* body;

public:
	std::string type;
	float x;
	float y;
	virtual ~Figura(){
        //this->world->DestroyBody(this->body);
	}

	bool validarOverlap();
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
	b2Vec2 GetCenter();
	b2Body* getBody();
	float getX();
	float getY();
	float getAngulo();
	float getMasa();
	bool esEstatico();


	virtual void reaccionarCon(Figura*){};

	virtual void reaccionarConCirculo(Circulo*){};
	virtual void reaccionarConMuro(Muro*){};
	virtual void reaccionarConParalelogramo(Paralelogramo*){};
	virtual void reaccionarConPoligono(Poligono*){};
	virtual void reaccionarConRectanguo(Rectangulo*){};
	virtual void reaccionarConTrapecio(Trapecio*){};
	virtual void reaccionarConEnemigo(Enemigo*){};
	virtual void reaccionarConPersonaje(Personaje*){};
	virtual void reaccionarConBolaNieve(BolaNieve*){};
	virtual void reaccionarConOtroEnemigo(Enemigo*){};

};

#endif /* FIGURA_H_ */
