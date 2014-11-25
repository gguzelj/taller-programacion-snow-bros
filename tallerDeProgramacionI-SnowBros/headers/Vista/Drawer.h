#ifndef DRAWER_H_
#define DRAWER_H_

#include "../Log/Log.h"
#include <iostream>
#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_ttf.h>
#include "../Exceptions/SDLError.h"
#include "../Exceptions/ErrorFueraDeRango.h"
#include "LTexture.h"
#include <cmath>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <iostream>
#include <arpa/inet.h>

#define ZOOM_MAX 1.7
#define BOLA_PORTAL_CODE 'a'
#define BOLA_NIEVE_CODE 'b'
#define BOLA_FUEGO_CODE 'f'
#define CIRCULO_CODE 'c'
#define PARALELOGRAMO_CODE 'p'
#define RECTANGULO_CODE 'r'
#define PLATAFORMAHIELO_CODE 'l'
#define TRAPECIO_CODE 't'
#define TRIANGULO_CODE '3'
#define CUADRADO_CODE '4'
#define PENTAGONO_CODE '5'
#define HEXAGONO_CODE '6'
#define PORTAL_CODE 'o'
#define BONUS_POTENCIA_CODE 'g'
#define BONUS_VELOCIDAD_CODE 'h'
#define BONUS_VIDA_CODE 'i'
#define BONUS_PORTAL_CODE 'j'

#define ID_ENEMIGO_BASICO 11
#define ID_ENEMIGO_FUEGO 12

//Conversion de unidades
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

//defines de la camara
#define FACTOR_CONVERSION_UN_A_PX 25
#define COTA_INF_X -(coordRel.w)/5
#define COTA_INF_Y -(coordRel.h)/5
#define COTA_SUP_X	(coordRel.w)/5
#define COTA_SUP_Y	(coordRel.h)/5

//defines que eran del Personaje.cpp
#define MITAD_ANCHO_PERSONAJE 0.65f
#define MITAD_ALTO_PERSONAJE 0.85f

//Mensaje de espera
#define WAITING_MSG "Esperando a todos los jugadores"
#define GAMEOVER_MSG "Game over"

//defines que eran de Estados.h
#define JUMPING 'j'
#define STANDBY 's'
#define WALKING 'w'
#define FALLING 'f'
#define SHOOTING 'd'
#define DYING 'm'
#define PUSHING 'p'
#define KICKING 'k'

#define FLYING 'y'


#define BALLBREAKING 'K'
#define GAMEOVER 'G'
#define BONUS 'B'
#define ONEUP 'O'
#define TRANSITION 'T'

#define COLOR_BLANCO 'b'
#define COLOR_AZUL 'a'
#define COLOR_VERDE 'v'
#define COLOR_ROJO 'r'

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
	bool inmune;
	char color;
}personaje_t;

typedef struct enemigo{
	int tipo;
	punto_t centro;
	float ancho;
	float alto;
	char estado;
	char orientacion;
	int nivelDeCongelamiento;
}enemigo_t;

typedef struct gameData{
	unsigned int nivel;
	bool paused;
	bool gameOver;
	bool won;
	unsigned int cantProyectiles;
	unsigned int cantEnemigos;
	unsigned int cantPersonajes;
	unsigned int cantDinamicos;
	unsigned int cantSonidos;
}gameData_t;

typedef struct dataFromClient{
	unsigned int cantPersonajes;
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
	gameData_t* gameData;
	personaje_t* personajes;
	enemigo_t* enemigos;
	figura_t* dinamicos;
	figura_t* estaticos;
	proyectil_t* proyectiles;
} dataFromClient_t;

class Drawer{
public:
	Drawer();
	~Drawer();

	void updateView(dataFromClient_t data,char* name);

	/*
	 * Reproduce los sonidos recibidos del servidor.
	 */
	void reproducirSonidos(int* &sonidos, unsigned int size);

	void inicializarCamara(personaje_t personaje);
	//Zooms in in a factor of 1.01x
	void zoomIn();
	//Zooms out in a factor of 0.99x
	void zoomOut();
	void setNivel(unsigned int nivel){
		if(!this->pasandoDeNivel){
			this->nivel = nivel;
			this->setearLimitesDelNivel(nivel);
		}
	}
	void transicionNivel();
	void finalizarTransicionNivel(){
		this->pasandoDeNivel = false;
	}


private:

	unsigned int nivel;

	int limIzqCamera ;
	int limDerCamera ;
	int limInfCamera ;
	int limSupCamera;

	int limiteIzquierdo ;
	int limiteDerecho;
	int limiteInferior ;
	int limiteSuperior ;

	bool pasandoDeNivel;
	bool seEstaReproduciendoMusicaDeFondo;
	bool seEstaReproduciendoMusicaDeVictoria;

	//SDL Attributes
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *image;
	SDL_Texture *waitingImage;
	SDL_Texture *winningImage;
	SDL_Texture* imagenPersonaje;
	SDL_Texture* imagenPersonaje2;
	SDL_Texture* imagenPersonaje3;
	SDL_Texture* imagenPersonaje4;
	SDL_Texture* imagenPersonaje5;
	SDL_Texture* imagenEnemigos;
	SDL_Texture* imagenEnemigoFuego;
	SDL_Texture* congelamientoUno;
	SDL_Texture* congelamientoDos;
	SDL_Texture* congelamientoTres;
	SDL_Texture* congelamientoCuatro;
	SDL_Texture* portal;

	SDL_Rect camera;
	SDL_Rect coordRel;
	TTF_Font* fontToBeUsed;

	//The music that will be played
	Mix_Music *gMusic;
	Mix_Chunk *gWinningMusic;
	Mix_Chunk *gShooting;
	Mix_Chunk *gJumping;
	Mix_Chunk *gDying;
	Mix_Chunk *gBallBreaking;
	Mix_Chunk *gBonus;
	Mix_Chunk *gOneUp;
	Mix_Chunk *gGameover;
	Mix_Chunk *gTransition;
	Mix_Chunk *gkicking;

	SDL_Color blanco;
	SDL_Color negro;

	//Scene texture
	LTexture rectangleLT;
	LTexture icePlatformLT;
	LTexture circleLT;
	LTexture triangleLT;
	LTexture squareLT;
	LTexture pentagonLT;
	LTexture hexagonLT;
	LTexture trapexLT;
	LTexture paralelogramLT;
	LTexture snowballLT;
	LTexture portalballLT;
	LTexture fireballLT;
	LTexture bonusPortalLT;
	LTexture bonusPotenciaLT;
	LTexture bonusVelocidadLT;
	LTexture bonusVidaLT;

	//Ahora los puntos, niveles y vidas son LTextures.
	LTexture pointsLT;
	LTexture livesLT;
	LTexture levelsLT;
	LTexture waitingScreenLT;
	LTexture winningScreenLT;
	LTexture gameOverScreenLT;

	//Cargo los numeros del 0 al 9
	LTexture ceroLT;
	LTexture unoLT;
	LTexture dosLT;
	LTexture tresLT;
	LTexture cuatroLT;
	LTexture cincoLT;
	LTexture seisLT;
	LTexture sieteLT;
	LTexture ochoLT;
	LTexture nueveLT;
	LTexture ceroBlackLT;
	LTexture unoBlackLT;
	LTexture dosBlackLT;

	//Cargo las letras minusculas
	LTexture aLT, bLT, cLT, dLT, eLT, fLT, gLT, hLT, iLT, jLT, kLT, lLT, mLT, nLT, oLT, pLT, qLT, rLT, sLT, tLT, uLT, vLT, wLT, xLT, yLT, zLT;
	LTexture auxLT;
	LTexture ALT, BLT, CLT, DLT, ELT, FLT, GLT, HLT, ILT, JLT, KLT, LLT, MLT, NLT, OLT, PLT, QLT,RLT, SLT,TLT, ULT, VLT, WLT, XLT, YLT, ZLT;

	std::vector<LTexture*> numerosLT;
	std::vector<LTexture*> numerosBlackLT;
	std::vector<LTexture*> letrasLT;

	//Paths Attributes
	string fontPath;
	string imagePath;
	string rectangleImage;
	string icePlatformImage;
	string circleImage;
	string triangleImagePath;
	string squareImagePath;
	string pentagonImagePath;
	string hexagonImagePath;
	string trapexImagePath;
	string paralelogramImagePath;
	string snowballImagePath;
	string portalballImagePath;
	string fireballImagePath;
	string bonusPortalPath;
	string bonusPotenciaPath;
	string bonusVelocidadPath;
	string bonusVidaPath;
	string portalPath;
	string gameOverScreenPath;
	string waitingScreenPath;
	string winningScreenPath;

	//Texto de puntos y vidas.
	string points;
	string lives;
	string level;

	//General Attributes
	std::vector<bool> personajeOn;
	std::vector<int> contadorOn;
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
	int anchoLevel;
	int altoText;
	int anchoNumber;
	int anchoWaiting;
	int altoWaiting;
	int puntaje;

	//Private Methods
	//General purpose methods
	void setearLimitesDelNivel(unsigned int nivel);
	void loadFont();
	bool loadMedia();
	void loadMusic();
	void runWindow(int ancho_px ,int alto_px ,string imagePath);
	void actualizarCamara(personaje_t personaje);
	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void setearLimiteInferiorDelNivel(unsigned int nivel);

	bool loadMusicAndSfx();
	void loadLetters();

	//Drawing methods
	void drawBackground();
	void drawScenary(dataFromClient_t data, char* name);
	void drawFigura(figura_t objeto);
	void drawProyectil(proyectil_t proyectil);
	void drawCharacter(personaje_t person, int index, int connectionState);
	void drawEnemy(enemigo_t enemigo);
	void drawMessages(dataFromClient_t data, personaje_t personaje);
	void drawWaitingScreen();
	void drawWinningScreen(dataFromClient_t data);
	void drawGameOverScreen();
	void presentScenary();
	void clearScenary();
	bool blinkCharacter(personaje_t person, int index);
	void drawStandardEnemy(enemigo_t enemigo);
	void drawFireEnemy(enemigo_t enemigo);

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

	/*
	* Draw an SDL_Texture to an SDL_Renderer at some destination rect
	* taking a clip of the texture if desired
	* @param tex The source texture we want to draw
	* @param ren The renderer we want to draw to
	* @param dst The destination rectangle to render the texture to
	* @param clip The sub-section of the texture to draw (clipping rect)
	*		default of nullptr draws the entire texture
	*/
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,SDL_Rect *clip);

	/*
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
	SDL_Texture* selectTexture(char color) throw(ErrorFueraDeRango);
};



#endif /* DRAWER_H_ */
