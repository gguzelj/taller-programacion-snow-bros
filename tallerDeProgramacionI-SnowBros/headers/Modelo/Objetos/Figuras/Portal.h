#ifndef HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_
#define HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_

#include "../Figura.h"

class Portal: public Figura {

private:
	float ancho;
	float alto;
	b2Vec2 destination;
	b2Vec2 address;

public:
	Portal(float ancho, float alto, int rot, b2Vec2 address, b2World* world);
	virtual ~Portal();

	void setDestination(const b2Vec2& destination);
	const b2Vec2& getDestination();

	void setAddress(const b2Vec2& Address);
	const b2Vec2& getAddress();

	void beginContact(Figura*, b2Contact*);

	float getAncho();
	float getAlto();
	char getId();

};

#endif /* HEADERS_MODELO_OBJETOS_FIGURAS_PORTAL_H_ */
