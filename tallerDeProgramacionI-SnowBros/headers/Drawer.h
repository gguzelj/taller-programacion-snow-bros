#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include <Box2D/Box2D.h>

using namespace std;

class Drawer{
	private:
	public:
		Drawer(){};
		virtual ~Drawer(){};
		virtual void drawBackground() = 0;
		virtual void drawStaticBody(b2Body* body) = 0;
		virtual void drawScenary(b2World* world) = 0;
		virtual void presentScenary() = 0;
		virtual void clearScenary() = 0;
};



#endif /* DRAWER_H_ */
