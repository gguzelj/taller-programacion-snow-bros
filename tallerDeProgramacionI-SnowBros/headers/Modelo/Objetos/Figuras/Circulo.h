#ifndef CIRCULO_H_
#define CIRCULO_H_

#include "Figura.h"

class Circulo : public Figura{
private:
	float escala;
public:
	Circulo();
	Circulo(int x, int y, float escala, std::string color, int angulo, float masa, bool estatico);
	~Circulo();
};

#endif; /* CIRCULO_H_ */
