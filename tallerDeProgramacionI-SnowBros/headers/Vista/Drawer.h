#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include <Box2D/Box2D.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../Parser/JsonParser.h"
#include "../Modelo/Escenario.h"
#include <cmath>

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
	int ancho_px;
	int alto_px;
	float alto_un;
	float ancho_un;
	float un_to_px_x;
	float un_to_px_y;

	void runWindow(int ancho_px ,int alto_px ,string imagePath);

	void drawBackground();
	void drawScenary(Escenario* model);
	void drawFigura(Figura* figura);
	void drawCharacter();
	void presentScenary();
	void clearScenary();

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void logSDLError(std::ostream &os, const std::string &msg);

};



#endif /* DRAWER_H_ */
