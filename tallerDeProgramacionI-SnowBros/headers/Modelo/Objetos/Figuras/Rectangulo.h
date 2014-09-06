#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "Figura.h"

class Rectangulo : public Figura {
private:
	int ancho;
	int alto;
public:
	Rectangulo(int x, int y, int ancho, int alto, std::string color, int angulo, float masa, bool estatico, b2World* world);
	Rectangulo();
	~Rectangulo();
};


#endif /* RECTANGULO_H_ */
