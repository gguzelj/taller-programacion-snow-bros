#ifndef DRAWER_H_
#define DRAWER_H_

#include "../Log/Log.h"
#include <iostream>
#include <Box2D/Box2D.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>
#include "../Exceptions/SDLError.h"
#include "../Exceptions/ErrorFueraDeRango.h"
#include "LTexture.h"
#include <cmath>
#include <typeinfo>
#include <fstream>
#include <iostream>
#include <arpa/inet.h>

#define ZOOM_MAX 1.7
#define BOLA_NIEVE_CODE 'b'
#define CIRCULO_CODE 'c'
#define PARALELOGRAMO_CODE 'p'
#define RECTANGULO_CODE 'r'
#define TRAPECIO_CODE 't'
#define TRIANGULO_CODE '3'
#define CUADRADO_CODE '4'
#define PENTAGONO_CODE '5'
#define HEXAGONO_CODE '6'

//Conversion de unidades
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

//defines de la camara
#define FACTOR_CONVERSION_UN_A_PX 25
#define COTA_INF_X -(ancho_px )/5
#define COTA_INF_Y -(alto_px)/5
#define COTA_SUP_X	(ancho_px)/5
#define COTA_SUP_Y	(alto_px)/5

//defines que eran del Personaje.cpp
#define MITAD_ANCHO_PERSONAJE 0.5f
#define MITAD_ALTO_PERSONAJE 0.85

//defines que eran de Estados.h
#define JUMPING 'j'
#define STANDBY 's'
#define WALKING 'w'
#define FALLING 'f'

using namespace std;

const float factor = 0.005;
const float rotation = 0.0;
const int smoothing_off = 0;

typedef struct punto{
	float x;
	float y;
} punto_t;

typedef struct figura{
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} figura_t;

typedef struct proyectil{
	punto_t centro;
	char id;
	float ancho;
	float alto;
	float rotacion;
} proyectil_t;

typedef struct personaje{
	punto_t centro;
	char id[20];
	float ancho;
	float alto;
	char estado;
	char orientacion;
	char connectionState;
	int points;
	int lives;
}personaje_t;

typedef struct enemigo{
	punto_t centro;
	float ancho;
	float alto;
	char estado;
	char orientacion;
}enemigo_t;

typedef struct gameData{
	bool paused;
	int cantProyectiles;
}gameData_t;

typedef struct dataFromClient{
	unsigned int cantPersonajes;
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
	gameData_t* gameData;
	personaje_t* personajes;
	figura_t* dinamicos;
	figura_t* estaticos;
	proyectil_t* proyectiles;
} dataFromClient_t;

class Drawer{
public:
	Drawer();
	~Drawer();

	void updateView(dataFromClient_t data,char* name);
	void inicializarCamara(personaje_t personaje);
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
	SDL_Texture* imagenPersonaje2;
	SDL_Texture* imagenPersonaje3;
	SDL_Texture* imagenPersonaje4;
	SDL_Texture* imagenPersonaje5;
	SDL_Texture* messageAboutPoints;
	SDL_Texture* messageAboutLifes;
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
	//Ahora los puntos y vidas son LTextures.
	LTexture pointsT;
	LTexture livesT;
	LTexture waitingScreenT;

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
	//Texto de puntos y vidas.
	string points;
	string lives;

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
	int anchoLives;
	int anchoPoints;
	int altoText;

	//Private Methods
	//General purpose methods
	void loadFont();
	bool loadMedia();
	void runWindow(int ancho_px ,int alto_px ,string imagePath);
	void actualizarCamara(personaje_t personaje);
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

	//Drawing methods
	void drawBackground();
	void drawScenary(dataFromClient_t data, char* name);
	void drawFigura(figura_t objeto);
	void drawProyectil(proyectil_t proyectil);
	void drawCharacter(personaje_t person, int index, int connectionState);
	void drawMessages(dataFromClient_t data, personaje_t personaje);
	void drawWaitingScreen();
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

	/*
	 * Selecciona una textura en base a un indice que corresponde a cada jugador.
	 * Este indice no puede ser negativo, ni tampoco > = 5
	 */
	SDL_Texture* selectTexture(int index) throw(ErrorFueraDeRango);
};



#endif /* DRAWER_H_ */
