#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <cstdint>
#include <unistd.h>
#include <thread>

#include "../../../Controlador/ControladorDeContactos.h"
#include "../../Estados/Estados.h"
#include "../Proyectiles/BolaNieve.h"

#include "../Figura.h"

#define IZQUIERDA 'l'
#define DERECHA 'r'

#define PERSONAJE_FILTER_INDEX -1
#define ENEMIGO_FILTER_INDEX -2

class Character: public Figura {
protected:
	b2Vec2 posicion;
	b2World* world;
	b2Body* body;
	int jumpCooldown;
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
	CharacterState* state;

	virtual ~Character() {
	}
	b2Vec2 Posicion();
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

	float getX();
	float getY();
	float getAncho() {
		return ancho;
	}
	float getAlto() {
		return alto;
	}
	void moveLeft();
	void moveRight();
	virtual void jump(){

	}
	void stop();
	void decreaseJumpCooldown();
	int getJumpCooldown();
	void updateLeftContact(int);
	void updateRightContact(int);
	virtual void handleInput(SDL_Keycode input, Uint32 input_type) {
	}

	b2Body* getb2Body() {
		return this->body;
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

	void terminoContacto(b2Fixture* fixture);
	void empiezoContacto(b2Fixture* fixture);
};

#endif /* CHARACTER_H_ */
