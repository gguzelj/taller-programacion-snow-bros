#ifndef BONUSAUMENTARPOTENCIA_H_
#define BONUSAUMENTARPOTENCIA_H_

#include "Bonus.h"

class BonusAumentarPotencia: public Bonus {

public:
	BonusAumentarPotencia(float, float, b2World*);
	~BonusAumentarPotencia();

	char getId();
	void beginContact(Figura* figura, b2Contact* contact);

};

#endif  /* BONUSAUMENTARPOTENCIA_H_ */
