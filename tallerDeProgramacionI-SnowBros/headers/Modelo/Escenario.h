#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <list>
#include <Box2D/Box2D.h>
#include "../Parser/JsonParser.h"
#include "Objetos/Figuras/Figura.h"

class Escenario {
public:
        Escenario(JsonParser *parser);
        virtual ~Escenario();

        std::list<Figura> getFiguras();

private:
	b2World* world_;
};

#endif /* ESCENARIO_H_ */
