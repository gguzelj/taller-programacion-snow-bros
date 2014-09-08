#ifndef PARALELOGRAMO_H_
#define PARALELOGRAMO_H_

#include <Box2D/Box2D.h>
#include "Figura.h"

class Paralelogramo : public Figura{
private:
	int ancho;
	int alto;
	int inclinacion;
public:
	Paralelogramo(int x, int y, int ancho, int alto, int inclinacion, bool inclinacionHaciaDerecha, std::string color, int angulo, float masa, bool estatico, b2World* world);
	~Paralelogramo();
};


#endif /* PARALELOGRAMO_H_ */
