#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include <Box2D/Box2D.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include "../Parser/JsonParser.h"
#include "../Modelo/Escenario.h"
#include "../Exceptions/SDLError.h"
#include <cmath>

using namespace std;

const float factor = 0.005;
const float rotation = 0.0;
const int smoothing_off = 0;



class Drawer{
public:
		Drawer(JsonParser *parser);
		~Drawer();

		void updateView(Escenario* model_);

		//Zooms in in a factor of 1.01x
		void zoomIn();
		//Zooms out in a factor of 0.99x
		void zoomOut();

private:
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *image;
	SDL_Surface* surfaceBackground;
	string imagePath;
	int ancho_px;
	int alto_px;
	float alto_un;
	float ancho_un;
	float un_to_px_x;
	float un_to_px_y;
	float un_to_px_x_inicial;
	float un_to_px_y_inicial;
	float currentZoomFactor;
	SDL_Texture* imagenPersonaje;
	SDL_Rect camera;
	SDL_Rect coordRel;

	void runWindow(int ancho_px ,int alto_px ,string imagePath);

	void actualizarCamara(Personaje* personaje);

	void drawBackground();
	void drawScenary(Escenario* model);
	void drawFigura(Figura* figura);
	void drawCharacter(Personaje* person);
	void presentScenary();
	void clearScenary();

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void logSDLError( const std::string &msg);
	void manageSDL2_imageError();
	void manageCreateWindowError();
	void manageCreateRendererError();
	void manageLoadBackgroundError();
	void manageLoadCharacterError();
};



#endif /* DRAWER_H_ */
