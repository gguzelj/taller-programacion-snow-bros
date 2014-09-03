/*
 * Drawer.h
 *
 *  Created on: Sep 2, 2014
 *      Author: federico
 */

#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <Box2D/Box2D.h>
#include "../headers/constants.h"
#include "../headers/ventanaGrafica.h"
using namespace std;

class Drawer{
	private:
	public:
		Drawer();
		~Drawer();
		void drawStaticBody(SDL_Renderer* renderer, b2Body* body);
		void drawScenary(b2World* world, VentanaGrafica* window);
};



#endif /* DRAWER_H_ */
