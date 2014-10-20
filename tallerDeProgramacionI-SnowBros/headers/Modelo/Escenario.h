#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#define CIRCULO_CODE 'c'
#define PARALELOGRAMO_CODE 'p'
#define RECTANGULO_CODE 'r'
#define TRAPECIO_CODE 't'
#define TRIANGULO_CODE '3'
#define CUADRADO_CODE '4'
#define PENTAGONO_CODE '5'
#define HEXAGONO_CODE '6'

#include <list>
#include <vector>
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

typedef char conn_id[20];
typedef struct punto{
	float x;
	float y;
} punto_t;

typedef struct personaje{
	punto_t centro;
	char id[20];
	float ancho;
	float alto;
	float rotacion;
	char estado;
	char orientacion;
}personaje_t;

typedef struct figura{
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} figura_t;

class Escenario {
public:
	Escenario(JsonParser *parser);
	virtual ~Escenario();

	std::list<Muro*>* getParedes();

	//Devuelve una instancia al personaje del juego
	std::list<Personaje*>* getPersonajes();

	bool crearPersonaje(float x, float y,conn_id id);

	b2World* getWorld();

	unsigned int getCantObjDinamicos();
	unsigned int getCantObjEstaticos();
	unsigned int getCantPersonajes();

	personaje_t* getPersonajesParaEnvio();

	figura_t* getObjetosEstaticos();
	figura_t* getObjetosDinamicos();

	/*
	 * Devuelve la lista de Personajes
	 */
	Personaje* getPersonaje(conn_id id);

	/*
	 * Realiza un step en la simulacion.
	 */
	void step();

	float getAnchoUn();

private:

	const float32 timeStep = 1 / 60.0; //the length of time passed to simulate (seconds)
	const int32 velocityIterations = 32;   //how strongly to correct velocity
	const int32 positionIterations = 64;   //how strongly to correct position}

	b2World* world_;
	std::vector<Figura*>* figurasEstaticas_;
	std::vector<Figura*>* figurasDinamicas_;
	std::list<Muro*>* muros_;
	std::list<Personaje*>* personajes_;

	float ancho_un;
	float alto_un;
};

#endif /* ESCENARIO_H_ */
