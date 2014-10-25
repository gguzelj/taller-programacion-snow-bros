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

#define ALTO_CAMINAR 26
#define POS_X_CAMINAR 30
#define POS_Y_CAMINAR 10

#define ANCHO_DEFAULT 30
#define ALTO_DEFAULT 30
#define FRAMES_CAMINAR 3

#define FRAMES_SALTAR 5
#define POS_X_SALTAR 120

#define POS_X_STANDBY 0
#define POS_Y_STANDBY 10

#define POS_X_CAYENDO 112
#define POS_Y_CAYENDO 10

//orientacion l = izquierda ; r = derecha;
void drawPersonajeCaminando(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//con 150 me parece que se ve bien, aumentar para verlo mas lendo o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 150  ) % FRAMES_CAMINAR;
	//defino srcrect posee la info del frame a dibujar
	SDL_Rect srcrect = { POS_X_CAMINAR + (sprite *ANCHO_DEFAULT) ,POS_Y_CAMINAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

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

void drawPersonajeSaltando(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100 ) % FRAMES_SALTAR;

	SDL_Rect srcrect = { POS_X_SALTAR + (sprite *ANCHO_DEFAULT) ,POS_Y_CAMINAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

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

void drawPersonajeStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY ,POS_Y_STANDBY, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	switch (orientacion){
		case 'l':{
			SDL_RenderCopy(renderer,texture,&srcrect,&dstrect);
			break;
		}
		case 'r':{
			SDL_Point center = { pos_x , pos_y};
			SDL_RenderCopyEx(renderer,texture,&srcrect,&dstrect,0,&center,SDL_FLIP_HORIZONTAL);
			break;
		}
	 }
}

void drawPersonajeCayendo(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){


	SDL_Rect srcrect = {  POS_X_CAYENDO ,POS_Y_CAYENDO, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	switch (orientacion){
		case 'l':{
			SDL_RenderCopy(renderer,texture,&srcrect,&dstrect);
			break;
		}
		case 'r':{
			SDL_Point center = { pos_x , pos_y};
			SDL_RenderCopyEx(renderer,texture,&srcrect,&dstrect,0,&center,SDL_FLIP_HORIZONTAL);
			break;
		}
	}
}

