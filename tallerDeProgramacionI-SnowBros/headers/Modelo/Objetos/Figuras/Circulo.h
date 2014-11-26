#ifndef CIRCULO_H_
#define CIRCULO_H_

#include "../Figura.h"

class Circulo : public Figura{
private:
        float radio;
public:
        Circulo(JsonParser *parser, int index, b2World* world);
        ~Circulo();
    	bool esEstatico(){
    		return estatico;
    	}
        float getRadio();
        float getCoordX();
        float getCoordY();
    	float getAlto();
    	float getAncho();
    	char getId();
};

#endif /* CIRCULO_H_ */
