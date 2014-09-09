#ifndef POLIGONO_H_
#define POLIGONO_H_

#include "Figura.h"

class Poligono : public Figura{
private:
	float escala;
public:
	Poligono(JsonParser *parser, int index, b2World* world);
	Poligono();
	~Poligono();
};

#endif /* POLIGONO_H_ */
