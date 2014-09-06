#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include <Box2D/Box2D.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../Parser/JsonParser.h"
#include "../Modelo/Escenario.h"

using namespace std;

class Drawer{
public:
		Drawer(JsonParser *parser, Escenario* model);
		~Drawer();

		void runWindow(int ancho_px ,int alto_px ,string imagePath);
		void updateView(b2World* world);

private:
	string imagePath;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *image;
	Escenario *model;

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void logSDLError(std::ostream &os, const std::string &msg);

	void drawBackground();
	void drawStaticBody(b2Body* body);
	void drawScenary(b2World* world);
	void presentScenary();
	void clearScenary();

};



#endif /* DRAWER_H_ */
