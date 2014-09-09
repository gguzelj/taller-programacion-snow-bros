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
	Paralelogramo(JsonParser *parser, int index, b2World* world);
	~Paralelogramo();
};


#endif /* PARALELOGRAMO_H_ */
