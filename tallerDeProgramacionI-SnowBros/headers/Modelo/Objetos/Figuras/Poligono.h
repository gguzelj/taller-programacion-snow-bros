#ifndef POLIGONO_H_
#define POLIGONO_H_

#include "Figura.h"

class Poligono : public Figura{
private:
	float escala;
	int lados;
public:
	Poligono(JsonParser *parser, int index, b2World* world);
	Poligono();
	~Poligono();
	std::list<std::pair<float, float>*>* getListaVertices();
};

#endif /* POLIGONO_H_ */
