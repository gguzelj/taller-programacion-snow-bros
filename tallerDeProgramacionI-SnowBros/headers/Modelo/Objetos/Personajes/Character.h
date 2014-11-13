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
	b2Vec2 posicion;
	b2World* world;
	b2Body* body;
	int jumpCooldown;
	int shootCooldown;
	float ancho;
	float alto;
	int aceleracion;
	char orientacion;
	int lives;
	b2Fixture* paredIzquierda;
	b2Fixture* paredDerecha;
	b2Fixture* piso;

	void cambiarFilterIndex(int16 groupIndex);

public:
	bool inmune;
	bool movimientoLateralDerecha, movimientoLateralIzquierda, debeSaltar;
	int contactosActuales;
	int contactosIzquierda;
	int contactosDerecha;
	bool puedeEmpujar;

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

	virtual ~Character() {
	}
	b2Vec2 Posicion();
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

	float getX(){
		return (this->body->GetPosition().x);
	}

	float getY(){
		return (this->body->GetPosition().y);
	}

	float getAncho() {
		return ancho;
	}
	float getAlto() {
		return alto;
	}

	char getId(){
		return '0';
	}

	void moveLeft();
	void moveRight();
	void pushLeft();
	void pushRight();
	virtual void jump(){
	}
	void stop();
	void decreaseJumpCooldown();
	void decreaseShootCooldown();
	int getJumpCooldown();
	int getShootCooldown();
	void updateLeftContact(int);
	void updateRightContact(int);
	virtual void handleInput(SDL_Keycode input, Uint32 input_type) {
	}

	;
	b2Vec2 getVelocity();
	int getCantidadDeContactosActuales();
	char getOrientacion();
	void setOrientacion(char orientacion);

	virtual void disparar() {
	}

	virtual void sacarVida() {
	}

	virtual void empujar(){
	}

	void beginContact(Figura*,b2Contact*);

	void terminoContacto(b2Fixture* fixture);
	void empiezoContacto(b2Fixture* fixture);

	virtual void atravezarPlataformas();
	virtual void noAtravezarPlataformas(){};

	void test();
};

#endif /* CHARACTER_H_ */
