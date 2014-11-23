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

#define ANCHO_MURIENDO 40
#define ALTO_MURIENDO 32
#define FRAMES_MURIENDO 7
#define POS_X_MURIENDO 0
#define POS_Y_MURIENDO 200

#define FRAMES_EMPUJANDO 2
#define POS_X_EMPUJANDO 206
#define POS_Y_EMPUJANDO 51

#define FRAMES_PATEANDO 1
#define POS_X_PATEANDO 326
#define POS_Y_PATEANDO 48

#define FRAMES_PORTAL 8
#define POS_X_PORTAL 0
#define POS_Y_PORTAL 0
#define ANCHO_PORTAL 43
#define ALTO_PORTAL 64



#define POS_X_VOLANDO 245
#define POS_Y_VOLANDO 240
#define FRAMES_VOLANDO 2
#define ALTO_VOLANDO 32



#define ALTO_BONUS 11
#define ANCHO_BONUS 9

#define POS_X_BONUSVE 174
#define POS_Y_BONUSVE 275

#define POS_X_BONUSMR 165
#define POS_Y_BONUSMR 283

#define POS_X_BONUSAP 154
#define POS_Y_BONUSAP 299

/*
 * Todas las demas funciones llaman a esta para hacer los dibujos.
 */

void dibujarFrame(SDL_Renderer *renderer,SDL_Texture *texture,char orientacion, int pos_x,int pos_y,SDL_Rect* srcrect, SDL_Rect dstrect){
	switch(orientacion){
		case 'l':{
			SDL_RenderCopy(renderer,texture,srcrect,&dstrect);
			break;
		}
		case 'r':{
			SDL_Point center = { pos_x , pos_y};
			SDL_RenderCopyEx(renderer,texture, srcrect,& dstrect,0,&center,SDL_FLIP_HORIZONTAL);
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

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeJumping(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100 ) % FRAMES_SALTAR;

	SDL_Rect srcrect = { POS_X_SALTAR + (sprite *ANCHO_DEFAULT) ,POS_Y_CAMINAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeStandBy(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = { POS_X_STANDBY ,POS_Y_STANDBY, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeFalling(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	SDL_Rect srcrect = {  POS_X_CAYENDO ,POS_Y_CAYENDO, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho , alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeShooting(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){

	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_DISPARAR;

	SDL_Rect srcrect = { POS_X_DISPARAR + (sprite *ANCHO_DEFAULT) ,POS_Y_DISPARAR, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeMuriendo(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_MURIENDO;

	SDL_Rect srcrect = { POS_X_MURIENDO + (sprite * ANCHO_MURIENDO) ,POS_Y_MURIENDO, ANCHO_MURIENDO, ALTO_MURIENDO };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajeEmpujando(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_EMPUJANDO;

	SDL_Rect srcrect = { POS_X_EMPUJANDO + (sprite * ANCHO_DEFAULT) ,POS_Y_EMPUJANDO, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPersonajePateando(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_PATEANDO;

	SDL_Rect srcrect = { POS_X_PATEANDO + (sprite * ANCHO_DEFAULT) ,POS_Y_PATEANDO, ANCHO_DEFAULT, ALTO_DEFAULT };
	SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}

void drawPortal(SDL_Renderer *renderer,SDL_Texture *texture, int pos_x,int pos_y,int ancho,int alto){
	//Le tuve que agregar esto porque necesitaba si o si la orientacion para el otro metodo.
	const char orientacion = 'l';
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
	Uint32 ticks = SDL_GetTicks();
	//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
	Uint32 sprite = (ticks / 120  ) % FRAMES_PORTAL;

	SDL_Rect srcrect = { POS_X_PORTAL + (sprite * ANCHO_PORTAL) ,POS_Y_PORTAL, ANCHO_PORTAL, ALTO_PORTAL };
	SDL_Rect dstrect = { pos_x, pos_y, ancho, alto};

	dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);
}


void drawPersonajeVolando(SDL_Renderer *renderer,SDL_Texture *texture,const char orientacion, int pos_x,int pos_y,int ancho,int alto){
	//Get ticks te da el tiempo en milisegundo desde el arranque de la ejecucion.
		Uint32 ticks = SDL_GetTicks();
		//Aumentar para verlo mas lento o disminuir el parametro para verlo mas rapido.
		Uint32 sprite = (ticks / 120  ) % FRAMES_VOLANDO;

		SDL_Rect srcrect = { POS_X_VOLANDO + (sprite * ANCHO_DEFAULT) ,POS_Y_VOLANDO, ANCHO_DEFAULT, ALTO_VOLANDO };
		SDL_Rect dstrect = { pos_x - (ancho/2), pos_y -( alto/2), ancho, alto};

		dibujarFrame(renderer,texture,orientacion,pos_x,pos_y,&srcrect,dstrect);

}
