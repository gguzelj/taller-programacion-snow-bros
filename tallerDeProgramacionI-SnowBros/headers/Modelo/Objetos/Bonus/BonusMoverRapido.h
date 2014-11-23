#ifndef BONUSMOVERRAPIDO_H_
#define BONUSMOVERRAPIDO_H_

#include "Bonus.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class BonusMoverRapido: public Bonus {

public:
	BonusMoverRapido(float, float, b2World*);
	~BonusMoverRapido();

	char getId();
	void beginContact(Figura* figura, b2Contact* contact);

};

#endif  /* BONUSMOVERRAPIDO_H_ */
