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
#include "Objetos/Personaje.h"

class Escenario {
public:
        Escenario(JsonParser *parser);
        virtual ~Escenario();

        std::list<Figura*>* getFiguras();

        //Devuelve una instancia al personaje del juego
        Personaje* getPersonaje();

private:
	b2World* world_;
	std::list<Figura*>* figuras_;
	Personaje* person_;
};

#endif /* ESCENARIO_H_ */
