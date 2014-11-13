#ifndef PARALELOGRAMO_H_
#define PARALELOGRAMO_H_

#include "../Figura.h"

class Paralelogramo : public Figura{
private:
	float ancho;
	float alto;
	int inclinacion;
public:
	Paralelogramo(JsonParser *parser, int index, b2World* world);
	~Paralelogramo();

	float getAlto();
	float getAncho();
	char getId();

};


#endif /* PARALELOGRAMO_H_ */
