#ifndef CONTROLADORDECONTACTOS_H_
#define CONTROLADORDECONTACTOS_H_

#include "Box2D/Box2D.h"
#include <iostream>
#include <list>

class Personaje;
class Enemigo;
class Character;

const int ID_FOOT_SENSOR = 3;
const int ID_LEFT_WALL_SENSOR = 4;
const int ID_RIGHT_WALL_SENSOR = 5;

// http://www.iforce2d.net/b2dtut/collision-callbacks
class Contacto : public b2ContactListener{

public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};


#endif /* CONTROLADORDECONTACTOS_H_ */
