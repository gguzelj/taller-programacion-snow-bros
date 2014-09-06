#ifndef POLIGONO_H_
#define POLIGONO_H_

#include "Figura.h"

class Poligono : public Figura{
private:
	float escala;
public:
	Poligono(int x, int y, float escala, std::string color, int angulo, float masa, bool estatico);
	Poligono();
	~Poligono();
};

#endif /* POLIGONO_H_ */
