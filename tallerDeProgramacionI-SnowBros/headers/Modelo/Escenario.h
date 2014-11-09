#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#define BOLA_NIEVE_CODE 'b'
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

#include "Objetos/Personajes/Character.h"
#include "Objetos/Personajes/Personaje.h"
#include "Objetos/Personajes/Enemigo.h"

#include "Objetos/Figura.h"
#include "Objetos/Figuras/Circulo.h"
#include "Objetos/Figuras/Paralelogramo.h"
#include "Objetos/Figuras/Rectangulo.h"
#include "Objetos/Figuras/Poligono.h"
#include "Objetos/Figuras/Trapecio.h"
#include "Objetos/Figuras/Muro.h"

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
	char estado;
	char orientacion;
	char connectionState;
	int points;
	int lives;
	bool inmune;
}personaje_t;

typedef struct enemigo{
	punto_t centro;
	float ancho;
	float alto;
	char estado;
	char orientacion;
	int nivelDeCongelacion;
}enemigo_t;

typedef struct gameData{
	bool paused;
	unsigned int cantProyectiles;
	unsigned int cantEnemigos;
}gameData_t;

typedef struct figura{
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} figura_t;

typedef struct proyectil{
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

	std::list<Muro*>* getParedes();

	//Devuelve una instancia al personaje del juego
	std::list<Personaje*>* getPersonajes();

	void setPersonajeConnectionState(conn_id id, char state);
	bool crearPersonaje(float x, float y,conn_id id);
	bool asignarPersonaje(conn_id id);

	b2World* getWorld();

	unsigned int getCantObjDinamicos();
	unsigned int getCantObjEstaticos();
	unsigned int getCantPersonajes();
	unsigned int getCantEnemigos();
	unsigned int getCantProyectiles();

	personaje_t* getPersonajesParaEnvio();
	enemigo_t* getEnemigosParaEnvio();

	figura_t* getObjetosEstaticos();
	figura_t* getObjetosDinamicos();
	proyectil_t* getProyectiles();

	/*
	 * Devuelve la lista de Personajes
	 */
	Personaje* getPersonaje(conn_id id);

	/*
	 * Realiza un step en la simulacion.
	 */
	void step();

	bool crearEnemigo(float x, float y);

	void actualizarEnemigos();

	float getAnchoUn();

	void agregarProyectil(Proyectil* proy);

private:
	Contacto contactos;
	const float32 timeStep = 1 / 40.0; //the length of time passed to simulate (seconds)
	const int32 velocityIterations = 32;   //how strongly to correct velocity
	const int32 positionIterations = 64;   //how strongly to correct position}

	b2World* world_;
	std::vector<Figura*>* figurasEstaticas_;
	std::vector<Figura*>* figurasDinamicas_;
	std::list<Proyectil*>* proyectiles_;
	std::list<Muro*>* muros_;
	std::list<Personaje*>* personajes_;
	std::list<Enemigo*>* enemigos_;
	std::list<Enemigo*>* enemigosAEliminar_;

	void clean();
	void movimientoDelEnemigo(Enemigo* enemigo);

	float ancho_un;
	float alto_un;
	unsigned int cantidadMaximaDePersonajes;
	unsigned int cantidadMaximaDeEnemigos;
};

#endif /* ESCENARIO_H_ */
