#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "../Objetos/Figuras/Figura.h"

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* world; 			//Viene por parametro.
		b2Body* cuerpoDinamicoPersonaje;
		int movimientoIzquierda, movimientoDerecha, movimientoNada, velocidadLimite, aceleracion, frenado;
	public:
		Personaje(JsonParser *parser, b2World* world);
		~Personaje();	//Destructor del personaje
		void Moverse(int); 			//Alguna clase controlador o algo, hasta tal vez un int segun que tipo de movimiento se eligio
		void Saltar();
		b2Vec2 Posicion();
};


#endif /* PERSONAJE_H_ */
