#ifndef CIRCULO_H_
#define CIRCULO_H_

#include "Figura.h"

class Circulo : public Figura{
private:
	float radio;
public:
	Circulo(JsonParser *parser, int index, b2World* world);
	~Circulo();
	float getRadio();
};

#endif /* CIRCULO_H_ */
