#ifndef RECTANGULO_H_
#define RECTANGULO_H_

#include "../Figura.h"

class Rectangulo: public Figura {

private:
	bool muro;
	float ancho;
	float alto;

public:
	/*
	 * Este constructor se va a usar para todas las figuras rectangulo que vengan del json.
	 */
	Rectangulo(JsonParser *parser, int index, b2World* world);

	/*
	 * Este constructor se va a usar para los muros que se crean en el escenario.
	 */
	Rectangulo(float ancho, float alto, int rot, float pos_x, float pos_y, b2World* world);
	~Rectangulo();


	bool esEstatico(){
		return estatico;
	}

	bool esMuro();

	float getAncho();
	float getAlto();
	char getId();

	void beginContact(Figura*,b2Contact*);
};

#endif /* RECTANGULO_H_ */
