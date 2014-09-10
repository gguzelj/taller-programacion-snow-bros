#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "../Objetos/Figuras/Figura.h"

class Personaje : public Figura{
        private:
                b2Vec2 posicion;
                b2World* world;                         //Viene por parametro.
                b2Body* cuerpoDinamicoPersonaje;
                int movimientoIzquierda, movimientoDerecha, movimientoNada, velocidadLimite, aceleracion, frenado;
                void Saltar(b2Vec2 VelocidadActual);
                int stickiness;                 //Para mostrar un poco de inercia
                int duracionStickiness;
        enum movimientos {
            Mov_Nada,
            Mov_Izquierda,
            Mov_Derecha,
        };
                movimientos movimientoActual;
                movimientos movimientoAnterior; //Va a servir para no interrumpir el movimiento al momento del salto

        public:
                Personaje(JsonParser *parser, b2World* world);
                ~Personaje();   //Destructor del personaje
                void Moverse(); //Alguna clase controlador o algo, hasta tal vez un int segun que tipo de movimiento se eligio
                b2Vec2 Posicion();
                void Keyboard(unsigned char key); //Deberia estar aca o en otro lado?
};


#endif /* PERSONAJE_H_ */
