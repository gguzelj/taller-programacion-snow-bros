#include "SDL2/SDL.h"
#include "sprite.h"

#define ENEMY_SPRITE_PATH "resources/sprites/SnowBrosEnemigos.png"


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





void drawStandardEnemyWalking(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 150  ) % FRAMES_CAMINAR_ES;
	//defino srcrect posee la info del frame a dibujar
	SDL_Rect srcrect = { POS_X_CAMINAR_ES + (sprite *ANCHO_CAMINAR_ES) ,POS_Y_CAMINAR_ES, ANCHO_CAMINAR_ES, ALTO_CAMINAR_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}



void drawStandardEnemyJumping(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_SALTAR_ES  ,POS_Y_SALTAR_ES, ANCHO_SALTAR_ES, ALTO_SALTAR_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);


}

void drawStandardEnemyStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY_ES ,POS_Y_STANDBY_ES, ANCHO_STANDBY_ES, ALTO_STANDBY_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}

void drawStandardEnemyFalling(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	SDL_Rect srcrect = {  POS_X_CAYENDO_ES ,POS_Y_CAYENDO_ES, ANCHO_CAYENDO_ES, ALTO_CAYENDO_ES };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}

