#ifndef SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOBASICO_H_
#define SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOBASICO_H_

#include "Enemigo.h"

class EnemigoBasico: public Enemigo {
public:
	EnemigoBasico(JsonParser*, int index, Escenario*);
	virtual ~EnemigoBasico();
};

#endif /* SRC_MODELO_OBJETOS_PERSONAJES_ENEMIGOBASICO_H_ */
