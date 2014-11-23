#ifndef BONUS_H_
#define BONUS_H_

#include "../Figura.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class Bonus: public Figura {

protected:
	float alto;
	float ancho;
	bool activado;

public:

	Bonus(float posicionX, float posicionY, b2World* world);
	virtual ~Bonus(){}

	bool activo();
	void desactivar();

	float getAncho();
	float getAlto();
	char getId();

};


#endif  /* BONUS_H_ */
