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
		Drawer(JsonParser *parser);
		~Drawer();

		void updateView(Escenario* model_);

private:
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *image;
	string imagePath;

	void runWindow(int ancho_px ,int alto_px ,string imagePath);

	void drawBackground();
	void drawStaticBody(Figura* figura);
	void drawScenary(Escenario* model);
	void presentScenary();
	void clearScenary();

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void logSDLError(std::ostream &os, const std::string &msg);

};



#endif /* DRAWER_H_ */
