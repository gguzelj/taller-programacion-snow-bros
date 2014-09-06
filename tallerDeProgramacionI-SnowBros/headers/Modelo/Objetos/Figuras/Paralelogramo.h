#ifndef PARALELOGRAMO_H_
#define PARALELOGRAMO_H_

#include <Box2D/Box2D.h>
#include "Figura.h"

class Paralelogramo : public Figura{
private:
	int ancho;
	int alto;
	int inclinacion;
	b2Body* body;
public:
	Paralelogramo();
	Paralelogramo(int x, int y, int ancho, int alto, int inclinacion, std::string color, int angulo, float masa, bool estatico)
	~Paralelogramo();
};


#endif /* PARALELOGRAMO_H_ */
