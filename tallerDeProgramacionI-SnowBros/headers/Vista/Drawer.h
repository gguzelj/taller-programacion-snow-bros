#ifndef DRAWER_H_
#define DRAWER_H_

#include <iostream>
#include <Box2D/Box2D.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>
#include "../Parser/JsonParser.h"
#include "../Modelo/Escenario.h"
#include "../Exceptions/SDLError.h"
#include "LTexture.h"
#include <cmath>
#include <typeinfo>

using namespace std;

const float factor = 0.005;
const float rotation = 0.0;
const int smoothing_off = 0;



class Drawer{
public:
	Drawer(JsonParser *parser);
	~Drawer();

	void updateView(Escenario* model_);
	void inicializarCamara(Personaje* personaje);
	//Zooms in in a factor of 1.01x
	void zoomIn();
	//Zooms out in a factor of 0.99x
	void zoomOut();

private:

	//SDL Attributes
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *image;
	SDL_Texture* imagenPersonaje;
	SDL_Surface* messageAboutPoints;
	SDL_Surface* messageAboutLifes;
	SDL_Rect camera;
	SDL_Rect coordRel;
	TTF_Font* fontToBeUsed;

	//Scene texture
	LTexture rectangleTexture;
	LTexture circleTexture;
	LTexture triangleTexture;
	LTexture squareTexture;
	LTexture pentagonTexture;
	LTexture hexagonTexture;
	LTexture trapexTexture;
	LTexture paralelogramTexture;

	//Paths Attributes
	string fontPath;
	string imagePath;
	string rectangleImage;
	string circleImage;
	string triangleImagePath;
	string squareImagePath;
	string pentagonImagePath;
	string hexagonImagePath;
	string trapexImagePath;
	string paralelogramImagePath;

	//General Attributes
	int ancho_px;
	int alto_px;
	float alto_un;
	float ancho_un;
	float un_to_px_x;
	float un_to_px_y;
	float un_to_px_x_inicial;
	float un_to_px_y_inicial;
	float currentZoomFactor;

	//Private Methods
	//General purpose methods
	bool loadMedia();
	void runWindow(int ancho_px ,int alto_px ,string imagePath);
	void actualizarCamara(Personaje* personaje);
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

	//Drawing methods
	void drawBackground();
	void drawScenary(Escenario* model);
	void drawFigura(Figura* figura);
	void drawCharacter(Personaje* person);
	void drawMessages();
	void presentScenary();
	void clearScenary();

	//Exception management methods
	void logSDLError( const std::string &msg);
	void manageSDL2_imageError();
	void manageCreateWindowError();
	void manageCreateRendererError();
	void manageLoadBackgroundError();
	void manageLoadCharacterError();
	void manageSDL_ttfError();
	void manageSDL_ttfLoadFontError();
	void manageDrawMessagesError();

	/**
	* Draw an SDL_Texture to an SDL_Renderer at some destination rect
	* taking a clip of the texture if desired
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param dst The destination rectangle to render the texture to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,SDL_Rect *clip);
	/**
	* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
	* the texture's width and height and taking a clip of the texture if desired
	* If a clip is passed, the clip's width and height will be used instead of
	*	the texture's
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param x The x coordinate to draw to
	* @param y The y coordinate to draw to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);
};



#endif /* DRAWER_H_ */
