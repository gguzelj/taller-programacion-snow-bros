#include "Figuras/Figura.h"
#include <cstdint>
#include "../../Controlador/ControladorDeContactos.h"
#include "../Estados/Estados.h"

#ifndef CHARACTER_H_
#define CHARACTER_H_


class Character : public Figura {
protected:
	b2Vec2 posicion;
	b2World* world;                         //Viene por parametro.
	b2Body* body;
	int jumpCooldown;
	float ancho;
	float alto;
	int aceleracion;
	char orientacion;



public:



	bool movimientoLateralDerecha,
	movimientoLateralIzquierda,
	debeSaltar;
	int cantidadDeContactosActuales;
	int cantidadDeContactosIzquierda;
	int cantidadDeContactosDerecha;
	static JumpingState jumping;
	static FallingState falling;
	static WalkingState walking;
	static StandByState standby;
	static ShootingState shooting;
	CharacterState* state;


	virtual ~Character(){

	}


	b2Vec2 Posicion();
	b2Fixture* GetFixtureList();
	b2Vec2 GetWorldPoint(const b2Vec2& localPoint);

	float getX();
	float getY();
	//Y estos por que estan definidos aca?
	float getAncho(){
		return ancho;
	}
	float getAlto(){
		return alto;
	}
	void moveLeft();
	void moveRight();
	void jump();
	void stop();
	void decreaseJumpCooldown();
	int getJumpCooldown();
	void updateLeftContact(int);
	void updateRightContact(int);
	virtual void handleInput(SDL_Keycode input,Uint32 input_type){

	}

	b2Body* getb2Body(){
		return this->body;
	}
	b2Vec2 getVelocity();
	int getCantidadDeContactosActuales();
	char getOrientacion();
	void setOrientacion(char orientacion);



};

#endif /* CHARACTER_H_ */
