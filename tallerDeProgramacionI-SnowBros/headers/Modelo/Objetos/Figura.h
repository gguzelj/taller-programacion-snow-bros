#ifndef FIGURA_H_
#define FIGURA_H_

#define BOLA_NIEVE_CODE 'b'
#define CIRCULO_CODE 'c'
#define MURO_CODE 'm'
#define PARALELOGRAMO_CODE 'p'
#define RECTANGULO_CODE 'r'
#define TRAPECIO_CODE 't'
#define TRIANGULO_CODE '3'
#define CUADRADO_CODE '4'
#define PENTAGONO_CODE '5'
#define HEXAGONO_CODE '6'

#define ID_CIRCULO 1
#define ID_MURO 2
#define ID_PARALELOGRAMO 3
#define ID_POLIGONO 4
#define ID_RECTANGULO 5
#define ID_TRAPECIO 6
#define ID_ENEMIGO 7
#define ID_PERSONAJE 8
#define ID_BOLA_NIEVE 9

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
	int type;
	float x;
	float y;
	virtual ~Figura(){}

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
	virtual float getAlto() = 0;
	virtual float getAncho() = 0;
	virtual char getId() = 0;

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
