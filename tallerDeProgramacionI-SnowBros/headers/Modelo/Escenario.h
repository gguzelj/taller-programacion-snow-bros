#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include "../Parser/JsonParser.h"

class Escenario {
public:
        Escenario(JsonParser *parser);
        virtual ~Escenario();
};

#endif /* ESCENARIO_H_ */
