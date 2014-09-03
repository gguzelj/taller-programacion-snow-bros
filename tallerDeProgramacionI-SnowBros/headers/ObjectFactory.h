#ifndef OBJECTFACTORY_H_
#define OBJECTFACTORY_H_

#include <iostream>
#include <Box2D/Box2D.h>
#include "../headers/constants.h"
using namespace std;

class ObjectFactory{
	private:
	public:
		ObjectFactory();
		~ObjectFactory();
		b2World* crearb2World();
		void createPlatforms(b2World* world);
		void createStaticBody(b2World* world);
};


#endif /* OBJECTFACTORY_H_ */
