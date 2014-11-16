#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <cstdint>
#include <unistd.h>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../../../Controlador/ControladorDeContactos.h"
#include "../../Estados/Estados.h"
#include "../Proyectiles/BolaNieve.h"
#include "../Proyectiles/BolaEnemigo.h"

#include "../Figura.h"

#define IZQUIERDA 'l'
#define DERECHA 'r'

#define ENEMIGO_BOLA 2
#define PERSONAJE_FILTER_INDEX -1
#define ENEMIGO_FILTER_INDEX -2

class Character: public Figura {
protected:

	b2Body* body;
	b2Vec2 posicion;
	b2Fixture* paredIzquierda;
	b2Fixture* paredDerecha;
	b2Fixture* piso;
	b2World* world;

	char orientacion;

	int aceleracion;
	int jumpCooldown;
	int lives;
	int shootCooldown;

	float ancho;
	float alto;

	void cambiarFilterIndex(int16 groupIndex);
	void move();
	virtual void shoot();

public:
	bool inmune;
	int contactosIzquierda;
	int contactosDerecha;
	bool movimientoDerecha;
	bool movimientoIzquierda;
	bool movimientoDisparar;

	static JumpingState jumping;
	static FallingState falling;
	static WalkingState walking;
	static StandByState standby;
	static ShootingState shooting;
	static DyingState dying;
	static PushingState pushing;
	static KickingState kicking;
	static RollingState rolling;
	CharacterState* state;

	virtual ~Character();

	virtual void atravezarPlataformas();
	virtual void noAtravezarPlataformas();
	virtual void sacarVida();
	virtual void kick();
	virtual void jump();
	virtual void handleInput(SDL_Keycode input, Uint32 input_type);

	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);
	b2Vec2 getVelocity();
	b2Vec2 Posicion();

	bool detener(char orientacion);
	bool estaEnAire();

	void move(char orientacion);
	void controlarEstado();
	void dejarDisparar();
	void detectarEstado();
	void disparar();
	void moveLeft();
	void moveRight();
	void pushLeft();
	void pushRight();
	void stop();
	void decreaseJumpCooldown();
	void decreaseShootCooldown();
	void setOrientacion(char orientacion);

	/*
	 * Getters
	 */
	bool getDisparando();
	char getId();
	char getOrientacion();
	int getJumpCooldown();
	int getShootCooldown();
	float getAncho();
	float getAlto();
	float getX();
	float getY();

};

#endif /* CHARACTER_H_ */
