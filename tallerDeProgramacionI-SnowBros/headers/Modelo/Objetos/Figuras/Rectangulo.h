#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "Figura.h"

class Rectangulo : public Figura {
private:
	int ancho;
	int alto;
public:
	Rectangulo(JsonParser *parser, int index, b2World* world);

	//Utilizada para representar la superficie
	Rectangulo(JsonParser *parser, b2World* world);
	Rectangulo();
	~Rectangulo();
};


#endif /* RECTANGULO_H_ */
