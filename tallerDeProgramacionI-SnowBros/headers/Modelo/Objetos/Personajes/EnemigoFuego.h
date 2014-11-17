#ifndef SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_
#define SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_

#include "Enemigo.h"

class EnemigoFuego: public Enemigo {

protected:
	void shoot();

public:
	EnemigoFuego(JsonParser*, int index, Escenario*);
	virtual ~EnemigoFuego();

	void mover();

};

#endif /* SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_ */
