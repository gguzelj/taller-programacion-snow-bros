#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <Box2D/Box2D.h>
#include "Character.h"
#include "Enemigo.h"
#include "../Bonus/BonusVidaExtra.h"
#include "../Bonus/BonusMoverRapido.h"
#include "../Bonus/BonusAumentarPotencia.h"
#include "../Bonus/BonusBolaPortal.h"

#define MITAD_ANCHO_PERSONAJE 0.65f
#define MITAD_ALTO_PERSONAJE 0.85

#define DESCONECTADO -1
#define ESPERANDO 0
#define CONECTADO 1

#define JUMPCOOLDOWN 18
#define SHOOTCOOLDOWN 10
#define PORTALCOOLDOWN 50
#define KICKCOOLDOWN 12

#define TIEMPO_INMUNIDAD 5
#define DURACION_BONUS 15

#define VEL_NORMAL 10
#define VEL_RAPIDA 15
#define POT_NORMAL 1
#define POT_FUERTE 5

typedef char conn_id[20];

class Escenario;

class Personaje: public Character {
public:
	conn_id id;
	Portal *portal1;
	Portal *portal2;
	char color;

	Personaje(float x, float y, char id[], Escenario* escenario, char color);
	~Personaje();

	/*
	 * Se encarga de llamar al handleInput de su estado para resolver la accion
	 * a realizar en base al input de entrada
	 */
	void handleInput(SDL_Keycode input, Uint32 input_type);

	/*
	 * Aca definimos como reacciona el Personaje ante el contacto con
	 * una BolaEnemigo. La misma se pasa por parametro para que se pueda
	 * definir su comportamiento tambien
	 */
	void beginContactBolaEnemigo(BolaEnemigo*, b2Contact*);

	/*
	 * Definimos el comportamiento del personaje ante la reaccion con
	 * una bola de Fuego
	 */
	void beginContactBolaFuego(BolaFuego*, b2Contact*);

	/*
	 * Encargado de realizar las acciones pertinentes en caso de que el
	 * Personaje entre en contacto con un Enemigo
	 */
	void beginContactEnemigo(Enemigo*, b2Contact*);

	/*
	 * Encargado de realizar las acciones pertinentes en el caso en que el
	 * Personaje entra en contacto con un bonus de vida extra.
	 */
	void beginContactBonusVidaExtra(BonusVidaExtra*, b2Contact*);

	/*
	 * Encargado de realizar las acciones pertinentes en el caso en que el
	 * Personaje entra en contacto con un bonus de movimiento rapido.
	 */
	void beginContactBonusMoverRapido(BonusMoverRapido*, b2Contact*);

	/*
	 * Encargado de realizar las acciones pertinentes en el caso en que el
	 * Personaje entra en contacto con un bonus de Aumentar Potencia.
	 */
	void beginContactBonusAumentarPotencia(BonusAumentarPotencia*, b2Contact*);

	/*
	 * Encargado de realizar las acciones pertinentes en el caso en que el
	 * Personaje entra en contacto con un bonus de bola portal.
	 */
	void beginContactBonusBolaPortal(BonusBolaPortal*, b2Contact*);


	/*
	 * Realiza las corecciones pertinentes al estado del Personaje previo
	 * al step.
	 */
	void controlarEstado();

	/*
	 * Realiza la accion de disparar una BolaNieve
	 */
	void shoot();

	/*
	 * Se encarga de realizar la accion de patear y pasar al Personaje al
	 * estado correspondiente
	 */
	void kick();


	/*
	 * Se encarga de realizar la accion de saltar y pasar al Personaje al
	 * estado correspondiente
	 */
	void jump();

	/*
	 * Se encarga de setear que se debe disparar una bola portal
	 */
	void dispararPortal();

	/*
	 *
	 */
	void dejarDispararPortal();

	/*
	 * Se encarga de disparar una bola portal
	 */
	void shootPortal();

	/*
	 * Se encarga de realizar la accion de morir y pasar al Personaje al
	 * estado correspondiente
	 */
	void morir();

	/*
	 * Se encarga de aumentar la velocidad al Personaje durante un tiempo
	 * determinado. Luego vuelve a su velocidad normal.
	 */
	void aumentarVelocidad();

	/*
	 * Se encarga de aumentar la potencia de las bolas de nieve del personaje
	 * durante un tiempo determinado. Luego vuelve a su potencia normal.
	 */
	void aumentarPotencia();

	/*
	 * Devuelve el personaje a su posicion inicial en el escenario
	 */
	void volverAPosicionInicial();

	/*
	 * Cambia el filter group del Personaje para que el mismo no pueda atravesar
	 * plataformas
	 */
	void noAtravezarPlataformas();

	/*
	 * Resta una vida al Personaje
	 */
	void sacarVida();

	/*
	 * Disminuye el kickCooldown en una unidad
	 */
	void decreaseKickCooldown();

	/*
	 * Disminuye el shootPortalCooldown en una unidad
	 */
	void decreaseShootPortalCooldown();

	/*
	 * Setea el connectionState del Personaje al pasado por parametro
	 */
	void setConnectionState(char state);

	/*
	 * Setea el joint del Personaje al pasado por parametro
	 */
	void setJoint(b2RevoluteJoint* joint);

	/*
	 * Setea el atributo arrastrado del Personaje al bool pasado por parametro
	 */
	void setArrastrado(bool valor);

	/*
	 * Setea el atributo arrastradoPor del Personaje al valor pasado por parametro
	 */
	void setArrastradoPor(BolaEnemigo* bola);

	/*
	 * Devuelve el b2Body del Personaje
	 */
	b2Body* getb2Body();

	/*
	 * Devuelve el FixtureList asociado al body del Personaje
	 */
	b2Fixture* GetFixtureList();


	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

	/*
	 * Devuelve true si el Personaje es inmune, false en caso contrario
	 */
	bool getInmune();

	/*
	 * Devuelve el connectionState del Personaje
	 */
	char getConnectionState();

	/*
	 * Devuelve el id del Personaje
	 */
	char getId();

	/*
	 * Devuelve el kickCooldown del Personaje
	 */
	int getKickCooldown();

	/*
	 * Devuelve las vidas restantes del Personaje
	 */
	int getLives();

	/*
	 * Devuelve los puntos actuales del personaje
	 */
	int getPoints();

	/*
	 * Devuelve el joint del Personaje o nullptr si no hay ninguno
	 */
	b2Joint* getJoint();

	/*
	 * Devuelve la BolaEnemigo por la cual es arrastrado el Personaje
	 * o nullptr si no hay ninguno.
	 */
	BolaEnemigo* getArrastradoPor();

	/*
	 * Devuelve el atributo jumpcooldown
	 */
	int getJumpCooldown();

	/*
	 * Devuelve el atributo shootcooldown
	 */
	int getShootCooldown();

	/*
	 * Devuelve el atributo portalCooldown
	 */
	int getPortalCooldown();

	/*
	 * Devuelve true si el Personaje esta_muerto o false en caso contrario
	 */
	bool estaMuerto();

	/*
	 * Devuelve true si el Personaje esta siendo arrastrado por una BolaEnemigo
	 */
	bool esArrastrado();

	/*
	 * Devuelve true si el Personaje esta empujando algun enemigo
	 */
	bool estaEmpujandoEnemigo();

	/*
	 * Suma los puntos ganados
	 */
	void addPoints(int puntos);


	void setPosicionInicial(float x, float y){
		this->posicionInicial->x = x;
		this->posicionInicial->y = y;
	}

	b2Vec2 getPosicionInicial(){
		return *posicionInicial;
	}

	void entrarEnPeriodoDeInmunidad();


	void resetearPortales(){
		this->arma_portal = false;
		this->portal1 = nullptr;
		this->portal2 = nullptr;
	}

	bool esta_muerto;

private:
	Escenario *escenario_;
	b2Joint* joint;
	BolaEnemigo* arrastradoPor;
	b2Vec2* posicionInicial;
	bool arma_portal;
	bool arrastrado;
	char connectionState;
	int points;
	int potencia;
	int kickCooldown;
	int portalCooldown;
	bool disparar_portal;

	/*
	 * Hace que el Personaje sea inmune
	 */


	/*
	 * El Personaje no es afectado por los enemigos
	 */
	void hacerInmune();

	Proyectil* crearBolaPortal();
	Proyectil* crearBolaNieve();
};

#endif /* PERSONAJE_H_ */
