#ifndef SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_
#define SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_

#include "Enemigo.h"

class EnemigoFuego: public Enemigo {
public:
	EnemigoFuego(JsonParser*, int index, Escenario*);
	virtual ~EnemigoFuego();
};

#endif /* SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOFUEGO_H_ */
