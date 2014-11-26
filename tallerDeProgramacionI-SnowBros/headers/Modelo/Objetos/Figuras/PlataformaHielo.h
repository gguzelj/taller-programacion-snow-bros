#ifndef PLATAFORMAHIELO_H_
#define PLATAFORMAHIELO_H_

#include "../Figura.h"

class PlataformaHielo : public Figura {

private:
	float ancho;
	float alto;

public:
	/*
	 * Este constructor se va a usar para todas las figuras rectangulo que vengan del json.
	 */
	PlataformaHielo(JsonParser *parser, int index, b2World* world);
	~PlataformaHielo();

	bool esEstatico(){
		return estatico;
	}
	float getAncho();
	float getAlto();
	char getId();

	void beginContact(Figura*,b2Contact*);
};



#endif /* PLATAFORMAHIELO_H_ */
