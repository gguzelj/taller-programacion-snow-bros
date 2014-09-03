/*
 * ventanaGrafica.cpp
 *
 *  Created on: Aug 31, 2014
 *      Author: lmasello
 */

#include "../headers/ventanaGrafica.h"


VentanaGrafica::VentanaGrafica(){
	this->alto_px = ALTO_PX;
	this->ancho_px = ANCHO_PX;
	this->alto_un = ALTO_UN;
	this->ancho_un = ANCHO_UN;
	this->imagePath = defaultImagePath;
	this->window = nullptr;
	this->renderer = nullptr;
	this-> image = nullptr;

	this->inicializarSDL();
}

VentanaGrafica::VentanaGrafica(int alto_px ,int ancho_px ,float alto_un ,float ancho_un, string imagePath){
	this->alto_px = alto_px;
	this->ancho_px = ancho_px;
	this->alto_un = alto_un;
	this->ancho_un = ancho_un;
	this->imagePath = imagePath;
	this->window = nullptr;
	this->renderer = nullptr;
	this-> image = nullptr;

	this->inicializarSDL();
}

//Aca hay que ver temas referentes a errores.
void VentanaGrafica::inicializarSDL(){
	//Starting SDL2_IMAGE
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		throw;
	}
	//Opening a window
	window = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, ancho_px, alto_px, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		IMG_Quit();
		SDL_Quit();
		throw;
	}

	//Creating a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		SDL_DestroyWindow(window);
		logSDLError(std::cout, "CreateRenderer");
		IMG_Quit();
		SDL_Quit();
		throw;
	}

	//Loading the image
	SDL_Texture *image = this->loadTexture(this->imagePath, renderer);
	if (image == nullptr){
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		throw;
	}
}

//Aca hay que ver temas referentes a errores.
SDL_Texture* VentanaGrafica::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

void VentanaGrafica::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void VentanaGrafica::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

//Aca hay que ver temas referentes a errores.
void VentanaGrafica::reproducirVentana(){
	//Drawing the texture
	SDL_RenderCopy(renderer, image, NULL, NULL); //Se pasa NULL para que ocupe todo el renderer
}

void VentanaGrafica::logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

VentanaGrafica::~VentanaGrafica(){
	//Cleaning up
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();
}

