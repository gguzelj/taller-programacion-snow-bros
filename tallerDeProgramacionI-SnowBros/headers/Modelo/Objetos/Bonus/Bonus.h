#ifndef BONUS_H_
#define BONUS_H_

#include "../Figura.h"

/*
 * Clase abstracta de la cual heredan los distintos tipos de bonus a usar.
 */
class Bonus: public Figura {

protected:
	bool activado; //Con este atributo se determinara si hay que sacar o no al bonus del escenario.

public:
	virtual ~Bonus(){}
	void desactivar();
	bool activo();

};


#endif  /* BONUS_H_ */
