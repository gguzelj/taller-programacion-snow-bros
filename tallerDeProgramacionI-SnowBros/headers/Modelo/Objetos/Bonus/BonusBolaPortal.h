#ifndef BONUSBOLAPORTAL_H_
#define BONUSBOLAPORTAL_H_

#include "Bonus.h"

class BonusBolaPortal: public Bonus {

public:
	BonusBolaPortal(float, float, b2World*);
	~BonusBolaPortal();

	char getId();
	void beginContact(Figura* figura, b2Contact* contact);

};

#endif  /* BONUSBOLAPORTAL_H_ */
