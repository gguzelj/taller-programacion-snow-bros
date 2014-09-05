#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include <Box2D/Box2D.h>
#include "../Objetos/Figuras/Figura.h"
#include "../../constants.h"

class Personaje : public Figura{
	private:
		b2Vec2 posicion;
		b2World* mundoDeJuego; //Se crea aca o ya viene de otro lado?
		b2Body* cuerpoDinamicoPersonaje;
		int movimientoIzquierda, movimientoDerecha, movimientoNada, velocidadLimite, aceleracion, frenado;
	public:
		Personaje();
		~Personaje(); //Destructor del personaje
		void Moverse(int); //Alguna clase controlador o algo, hasta tal vez un int segun que tipo de movimiento se eligio
		void Saltar();
		b2Vec2 Posicion();
};


#endif /* PERSONAJE_H_ */
