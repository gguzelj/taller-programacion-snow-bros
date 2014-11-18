#include "SDL2/SDL.h"
#include "sprite.h"

#define ENEMIGO_FUEGO_SPRITE_PATH "resources/sprites/enemigoFuego.png"
#define ENEMY_SPRITE_PATH "resources/sprites/SnowBrosEnemigos.png"
#define CONGELAMIENTO_NIVEL_UNO_PATH "resources/textures/snowball_uno.png"
#define CONGELAMIENTO_NIVEL_DOS_PATH "resources/textures/snowball_dos.png"
#define CONGELAMIENTO_NIVEL_TRES_PATH "resources/textures/snowball_tres.png"
#define CONGELAMIENTO_NIVEL_CUATRO_PATH "resources/textures/snowball.png"

#define ALTO_CAMINAR_ES 17
#define ANCHO_CAMINAR_ES 20
#define FRAMES_CAMINAR_ES 2
#define POS_X_CAMINAR_ES 0
#define POS_Y_CAMINAR_ES 0

#define ALTO_SALTAR_ES 21
#define ANCHO_SALTAR_ES 17
#define POS_X_SALTAR_ES 43
#define POS_Y_SALTAR_ES 18

#define ALTO_CAYENDO_ES 21
#define ANCHO_CAYENDO_ES 18
#define POS_X_CAYENDO_ES 43
#define POS_Y_CAYENDO_ES 39

#define ALTO_STANDBY_ES 17
#define ANCHO_STANDBY_ES 18
#define POS_X_STANDBY_ES 40
#define POS_Y_STANDBY_ES 0

#define ALTO_STANDBY_ES 17
#define ANCHO_STANDBY_ES 18
#define POS_X_STANDBY_ES 40
#define POS_Y_STANDBY_ES 0

// Parametros del enemigo fuego

#define ALTO_CAMINAR_FUEGO 90
#define ANCHO_CAMINAR_FUEGO 50
#define FRAMES_CAMINAR_FUEGO 5
#define POS_X_CAMINAR_FUEGO 8
#define POS_Y_CAMINAR_FUEGO 206

#define ALTO_STANDBY_FUEGO 90
#define ANCHO_STANDBY_FUEGO 50
#define POS_X_STANDBY_FUEGO 8
#define POS_Y_STANDBY_FUEGO 206

#define ALTO_CAYENDO_FUEGO 90
#define ANCHO_CAYENDO_FUEGO 44
#define POS_X_CAYENDO_FUEGO 10
#define POS_Y_CAYENDO_FUEGO 205

#define ALTO_DISPARANDO_FUEGO 90
#define ANCHO_DISPARANDO_FUEGO 44
#define POS_X_DISPARANDO_FUEGO 120
#define POS_Y_DISPARANDO_FUEGO 210

void drawCongelamiento(SDL_Renderer *renderer,SDL_Texture *texture, int pos_x,int pos_y,int ancho,int alto){
	SDL_Rect dstrect = { pos_x - ancho / 2, pos_y - alto / 2, ancho, alto};
	dibujarFrame(renderer,texture,'l',pos_x,pos_y,nullptr,dstrect);
}

void drawStandardEnemyWalking(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 150  ) % FRAMES_CAMINAR_ES;
	//defino srcrect posee la info del frame a dibujar
	SDL_Rect srcrect = { POS_X_CAMINAR_ES + (sprite *ANCHO_CAMINAR_ES) ,POS_Y_CAMINAR_ES, ANCHO_CAMINAR_ES, ALTO_CAMINAR_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawStandardEnemyJumping(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_SALTAR_ES  ,POS_Y_SALTAR_ES, ANCHO_SALTAR_ES, ALTO_SALTAR_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawStandardEnemyStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY_ES ,POS_Y_STANDBY_ES, ANCHO_STANDBY_ES, ALTO_STANDBY_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawStandardEnemyFalling(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = {  POS_X_CAYENDO_ES ,POS_Y_CAYENDO_ES, ANCHO_CAYENDO_ES, ALTO_CAYENDO_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawFireEnemyWalking(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 150  ) % FRAMES_CAMINAR_FUEGO;
	//defino srcrect posee la info del frame a dibujar
	SDL_Rect srcrect = { POS_X_CAMINAR_FUEGO + (sprite *ANCHO_CAMINAR_FUEGO) ,POS_Y_CAMINAR_FUEGO, ANCHO_CAMINAR_FUEGO, ALTO_CAMINAR_FUEGO };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawFireEnemyStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY_FUEGO ,POS_Y_STANDBY_FUEGO, ANCHO_STANDBY_FUEGO, ALTO_STANDBY_FUEGO };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawFireEnemyFalling(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = {  POS_X_CAYENDO_FUEGO ,POS_Y_CAYENDO_FUEGO, ANCHO_CAYENDO_FUEGO, ALTO_CAYENDO_FUEGO };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawFireEnemyShooting(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = {  POS_X_DISPARANDO_FUEGO ,POS_Y_DISPARANDO_FUEGO, ANCHO_DISPARANDO_FUEGO, ALTO_DISPARANDO_FUEGO };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}
