#ifndef BONUSMOVERRAPIDO_H_
#define BONUSMOVERRAPIDO_H_

#include "Bonus.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class BonusMoverRapido : public Bonus{

private:
	float alto, ancho;


public:
	BonusMoverRapido(float posicionX, float posicionY, b2World* world);
	~BonusMoverRapido();

	void desactivar();
	bool activo();
	float getAncho();
	float getAlto();
	char getId();


};


#endif  /* BONUSMOVERRAPIDO_H_ */
