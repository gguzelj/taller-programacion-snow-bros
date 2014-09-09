#ifndef CIRCULO_H_
#define CIRCULO_H_

#include "Figura.h"

class Circulo : public Figura{
private:
	float radio;
public:
	Circulo();
	Circulo(JsonParser *parser, int index, b2World* world);
	~Circulo();
};

#endif /* CIRCULO_H_ */
