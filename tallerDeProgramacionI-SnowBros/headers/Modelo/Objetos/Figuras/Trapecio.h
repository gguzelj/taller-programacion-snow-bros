#ifndef TRAPECIO_H_
#define TRAPECIO_H_

#include "../Figura.h"

class Trapecio : public Figura{
private:
	float base_inf;
	float base_sup;
	float alto;
	int inclinacion;
public:
	Trapecio(JsonParser *parser, int index, b2World* world);
	~Trapecio();
	float getBaseMayor();
	float getAlto();
	float getAncho();
	char getId();

	void reaccionarCon(Figura* figura){ ; };
};

#endif /* TRAPECIO_H_ */
