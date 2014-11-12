#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "../Figura.h"

class Rectangulo: public Figura {

private:
	float ancho;
	float alto;

public:
	Rectangulo(JsonParser *parser, int index, b2World* world);
	Rectangulo(float ancho, float alto, int rot, float pos_x, float pos_y, b2World* world);
	~Rectangulo();
	float getAncho();
	float getAlto();
	char getId();

};

#endif /* RECTANGULO_H_ */
