#ifndef HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_
#define HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_

#include "../Figura.h"

class Portal: public Figura {

private:
	float ancho;
	float alto;
	b2Vec2 destination;

public:
	Portal(float ancho, float alto, int rot, float pos_x, float pos_y, b2World* world);
	virtual ~Portal();

	void setDestination(const b2Vec2& destination);
	const b2Vec2& getDestination();

	void beginContact(Figura*,b2Contact*);

	float getAncho();
	float getAlto();
	char getId();

};

#endif /* HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_ */
