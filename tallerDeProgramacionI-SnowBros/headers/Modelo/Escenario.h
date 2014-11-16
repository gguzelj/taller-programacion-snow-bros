#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#define ASIGNADO(id) (strcmp((id), "sin asignar")) == 0
#define ALTURA_DEL_PISO -30 //Alto de la imagen / factor de conversion un to px
#define ALTURA_DEL_TECHO 0
#define ANCHO_DEL_PISO 60
#define ALTO_DE_PARED 60
#define LIMITE_PARED 28 //Ancho de la imagen / (factor de conversion*2)

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

	/*
	 * Metodos encargados de preparar la informacion a enviar a los clientes.
	 */
	enemigo_t* getEnemigosParaEnvio();
	figura_t* getObjetosEstaticos();
	figura_t* getObjetosDinamicos();
	figura_t* getFiguras(std::vector<Figura*>*);
	personaje_t* getPersonajesParaEnvio();
	proyectil_t* getProyectiles();

	/*
	 * Devuelve la lista de Personajes
	 */
	std::list<Personaje*>* getPersonajes();

	/*
	 * Devuelve el Personaje correspondiente al id
	 */
	Personaje* getPersonaje(conn_id id);

	/*
	 * Le asigna un Personaje al id
	 */
	bool asignarPersonaje(conn_id id);

	/*
	 * Crea un personaje en la posicion x,y con id
	 */
	bool crearPersonaje(float x, float y, conn_id id);

	/*
	 * Setea el connectionState state al personaje con tal id
	 */
	void setPersonajeConnectionState(conn_id id, char state);

	/*
	 * Crea un enemigo en la posicion x,y
	 */
	bool crearEnemigo(float x, float y);

	/*
	 * Agrega un proyectil a la lista de proyectiles
	 */
	void agregarProyectil(Proyectil* proy);

	/*
	 * Devuelve el ancho en unidades del escenario
	 */
	float getAnchoUn();

	/*
	 * Metodos que devuelven cantidad de objetos en el momento de ejecucion
	 */
	unsigned int getCantObjDinamicos();
	unsigned int getCantObjEstaticos();
	unsigned int getCantPersonajes();
	unsigned int getCantEnemigos();
	unsigned int getCantProyectiles();

	/*
	 * Actualiza la accion y por consecuente la posicion de cada enemigo en juego
	 */
	void actualizarEnemigos();

	/*
	 * Lleva a cabo la actualizacion de estados requerida antes de cada step
	 */
	void preStep();

	/*
	 * Realiza las acciones que incluyen el pre-step, el step propiamente dicho
	 * y las post actualizaciones de los Enemigos.
	 */
	void step();

private:
	Contacto contactos;
	const float32 timeStep = 1 / 40.0; 		//the length of time passed to simulate (seconds)
	const int32 velocityIterations = 32;    //how strongly to correct velocity
	const int32 positionIterations = 64;    //how strongly to correct position}

	b2World* world_;
	std::vector<Figura*>* figurasEstaticas_;
	std::vector<Figura*>* figurasDinamicas_;
	std::list<Proyectil*>* proyectiles_;
	std::list<Personaje*>* personajes_;
	std::list<Enemigo*>* enemigos_;

	/*
	 * Se encarga de la limpieza y eliminacion de objetos que deben ser removidos
	 */
	void clean();

	/*
	 * TODO y esto? No encuentro definicion del metodo.
	 */
	void movimientoDelEnemigo(Enemigo* enemigo);

	float ancho_un;
	float alto_un;
	unsigned int cantidadMaximaDePersonajes;
};

#endif /* ESCENARIO_H_ */
