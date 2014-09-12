#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <Box2D/Box2D.h>
#include "../Parser/JsonParser.h"
#include "Objetos/Figuras/Figura.h"
#include "Objetos/Figuras/Circulo.h"
#include "Objetos/Figuras/Paralelogramo.h"
#include "Objetos/Figuras/Rectangulo.h"
#include "Objetos/Figuras/Poligono.h"
#include "Objetos/Figuras/Trapecio.h"
#include "Objetos/Personaje.h"
#include "Objetos/Muro.h"

class Escenario {
public:
        Escenario(JsonParser *parser);
        virtual ~Escenario();

        std::list<Figura*>* getFiguras();

        std::list<Muro*>* getParedes();

        //Devuelve una instancia al personaje del juego
        Personaje* getPersonaje();

        b2World* getWorld();

private:
	b2World* world_;
	std::list<Figura*>* figuras_;
	std::list<Muro*>* muros_;
	Personaje* person_;
};

#endif /* ESCENARIO_H_ */
