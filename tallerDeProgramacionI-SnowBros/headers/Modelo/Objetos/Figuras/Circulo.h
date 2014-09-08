#ifndef CIRCULO_H_
#define CIRCULO_H_

#include "Figura.h"

class Circulo : public Figura{
private:
	float radio;
public:
	Circulo();
	Circulo(int posicionX, int posicionY, float radio, std::string color, int angulo, float masa, bool estatico, b2World* world);
	~Circulo();
};

#endif; /* CIRCULO_H_ */
