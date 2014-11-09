/*
 * sprite.h
 *
 *  Created on: 19/9/2014
 *      Author: anonimo
 */
#include "SDL2/SDL.h"

#define SPRITE_PATH "resources/sprites/sprite_player_own.png"
#define SPRITE2_PATH "resources/sprites/sprite_player_blue.png"
#define SPRITE3_PATH "resources/sprites/sprite_player_red.png"
#define SPRITE4_PATH "resources/sprites/sprite_player_green.png"
#define SPRITE5_PATH "resources/sprites/sprite_player_waiting.png"

#define ANCHO_DEFAULT 30
#define ALTO_DEFAULT 30

#define ALTO_CAMINAR 26
#define FRAMES_CAMINAR 3
#define POS_X_CAMINAR 30
#define POS_Y_CAMINAR 10

#define FRAMES_DISPARAR 3
#define POS_X_DISPARAR 3
#define POS_Y_DISPARAR 50

#define FRAMES_SALTAR 5
#define POS_X_SALTAR 120

#define POS_X_STANDBY 3
#define POS_Y_STANDBY 10

#define POS_X_CAYENDO 112
#define POS_Y_CAYENDO 10



void dibujarFrame(SDL_Renderer *renderer,SDL_Texture *texture,char orientacion, int pos_x,int pos_y,SDL_Rect srcrect, SDL_Rect dstrect){

	switch(orientacion){
		case 'l':{
			SDL_RenderCopy(renderer,texture,&srcrect,&dstrect);
			break;
		}
		case 'r':{
			SDL_Point center = { pos_x , pos_y};
			SDL_RenderCopyEx(renderer,texture,& srcrect,& dstrect,0,&center,SDL_FLIP_HORIZONTAL);
			break;
		}
	}
}




//orientacion l = izquierda ; r = derecha;
void drawPersonajeWalking(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 150  ) % FRAMES_CAMINAR;
	//defino srcrect posee la info del frame a dibujar
	SDL_Rect srcrect = { POS_X_CAMINAR + (sprite *ANCHO_DEFAULT) ,POS_Y_CAMINAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}

void drawPersonajeJumping(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100 ) % FRAMES_SALTAR;

	SDL_Rect srcrect = { POS_X_SALTAR + (sprite *ANCHO_DEFAULT) ,POS_Y_CAMINAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);


}

void drawPersonajeStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY ,POS_Y_STANDBY, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}

void drawPersonajeFalling(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	SDL_Rect srcrect = {  POS_X_CAYENDO ,POS_Y_CAYENDO, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}

void drawPersonajeShooting(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_DISPARAR;

	SDL_Rect srcrect = { POS_X_DISPARAR + (sprite *ANCHO_DEFAULT) ,POS_Y_DISPARAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,srcrect,dstrect);
}





