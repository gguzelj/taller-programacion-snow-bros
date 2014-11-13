#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#define ASIGNADO(id) (strcmp((id), "sin asignar")) == 0

#include <list>
#include <vector>
#include <Box2D/Box2D.h>
#include "../Parser/JsonParser.h"

#include "Objetos/Proyectiles/BolaEnemigo.h"

#include "Objetos/Personajes/Character.h"
#include "Objetos/Personajes/Personaje.h"
#include "Objetos/Personajes/Enemigo.h"

#include "Objetos/Figura.h"
#include "Objetos/Figuras/Circulo.h"
#include "Objetos/Figuras/Paralelogramo.h"
#include "Objetos/Figuras/Rectangulo.h"
#include "Objetos/Figuras/Poligono.h"
#include "Objetos/Figuras/Trapecio.h"

#include "../Exceptions/ErrorTipoDeObjeto.h"

typedef char conn_id[20];
typedef struct punto {
	float x;
	float y;
} punto_t;

typedef struct personaje {
	punto_t centro;
	char id[20];
	float ancho;
	float alto;
	char estado;
	char orientacion;
	char connectionState;
	int points;
	int lives;
	bool inmune;
} personaje_t;

typedef struct enemigo {
	punto_t centro;
	float ancho;
	float alto;
	char estado;
	char orientacion;
	int nivelDeCongelacion;
} enemigo_t;

typedef struct gameData {
	bool paused;
	unsigned int cantProyectiles;
	unsigned int cantEnemigos;
} gameData_t;

typedef struct figura {
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} figura_t;

typedef struct proyectil {
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} proyectil_t;

class Escenario {
public:

	Escenario(JsonParser *parser);
	virtual ~Escenario();

	b2World* getWorld();

	enemigo_t* getEnemigosParaEnvio();
	figura_t* getObjetosEstaticos();
	figura_t* getObjetosDinamicos();
	figura_t* getFiguras(std::vector<Figura*>*);
	Personaje* getPersonaje(conn_id id);
	personaje_t* getPersonajesParaEnvio();
	proyectil_t* getProyectiles();

	std::list<Personaje*>* getPersonajes();

	bool asignarPersonaje(conn_id id);
	bool crearEnemigo(float x, float y);
	bool crearPersonaje(float x, float y, conn_id id);

	float getAnchoUn();

	unsigned int getCantObjDinamicos();
	unsigned int getCantObjEstaticos();
	unsigned int getCantPersonajes();
	unsigned int getCantEnemigos();
	unsigned int getCantProyectiles();

	void actualizarEnemigos();
	void agregarProyectil(Proyectil* proy);
	void preStep();
	void setPersonajeConnectionState(conn_id id, char state);
	void step();

private:
	Contacto contactos;
	const float32 timeStep = 1 / 40.0; //the length of time passed to simulate (seconds)
	const int32 velocityIterations = 32;   //how strongly to correct velocity
	const int32 positionIterations = 64;   //how strongly to correct position}

	b2World* world_;
	std::vector<Figura*>* figurasEstaticas_;
	std::vector<Figura*>* figurasDinamicas_;
	std::list<Proyectil*>* proyectiles_;
	std::list<Personaje*>* personajes_;
	std::list<Enemigo*>* enemigos_;

	void clean();
	void movimientoDelEnemigo(Enemigo* enemigo);

	float ancho_un;
	float alto_un;
	unsigned int cantidadMaximaDePersonajes;
};

#endif /* ESCENARIO_H_ */
