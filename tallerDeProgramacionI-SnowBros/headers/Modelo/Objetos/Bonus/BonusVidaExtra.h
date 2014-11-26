#ifndef BONUSVIDAEXTRA_H_
#define BONUSVIDAEXTRA_H_

#include "Bonus.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class BonusVidaExtra: public Bonus {

public:
	BonusVidaExtra(float, float, b2World*);
	~BonusVidaExtra();

	bool esEstatico(){
		return false;
	}
	char getId();
	void beginContact(Figura* figura, b2Contact* contact);

};

#endif  /* BONUSVIDAEXTRA_H_ */
