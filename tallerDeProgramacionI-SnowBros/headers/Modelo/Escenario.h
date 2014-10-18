#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <Box2D/Box2D.h>
#include "../Parser/JsonParser.h"
#include "Objetos/Figuras/Figura.h"
#include "Objetos/Figuras/Circulo.h"
#include "Objetos/Figuras/Paralelogramo.h"
#include "Objetos/Figuras/Rectangulo.h"
#include "Objetos/Figuras/Poligono.h"
#include "Objetos/Figuras/Trapecio.h"
#include "Objetos/Personaje.h"
#include "Objetos/Muro.h"
#include "../Exceptions/ErrorTipoDeObjeto.h"

typedef struct punto{
	float x;
	float y;
} punto_t;

typedef struct objEstatico{
	punto_t centro;
	float ancho;
	float alto;
	float rotacion;
} objEstatico_t;

typedef struct objDinamico{
	punto_t centro;
	float ancho;
	float alto;
	float rotacion;
} objDinamico_t;

class Escenario {
public:
	Escenario(JsonParser *parser);
	virtual ~Escenario();

	std::list<Figura*>* getFiguras();

	std::list<Muro*>* getParedes();

	//Devuelve una instancia al personaje del juego
	std::list<Personaje*>* getPersonajes();

	bool crearPersonaje(float x, float y,char id[]);

	b2World* getWorld();

	unsigned int getCantObjDinamicos();
	unsigned int getCantObjEstaticos();
	objEstatico_t* getObjetosEstaticos();
	objDinamico_t* getObjetosDinamicos();

	void step();

private:

	const float32 timeStep = 1 / 60.0; //the length of time passed to simulate (seconds)
	const int32 velocityIterations = 32;   //how strongly to correct velocity
	const int32 positionIterations = 64;   //how strongly to correct position}

	b2World* world_;
	std::list<Figura*>* figuras_;
	std::vector<Figura*>* figurasEstaticas_;
	std::vector<Figura*>* figurasDinamicas_;
	std::list<Muro*>* muros_;
	std::list<Personaje*>* personajes_;
};

#endif /* ESCENARIO_H_ */
