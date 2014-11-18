#ifndef BONUSVIDAEXTRA_H_
#define BONUSVIDAEXTRA_H_

#include "Bonus.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class BonusVidaExtra : Bonus{

private:

public:
	BonusVidaExtra();
	~BonusVidaExtra();

	void desactivar();
	bool activo();
};


#endif  /* BONUSVIDAEXTRA_H_ */
