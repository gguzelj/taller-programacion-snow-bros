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
	this->imagePath = "resources/Everest.png";
	this->window = nullptr;
	this->renderer = nullptr;
	this-> image = nullptr;

	this->inicializarSDL();
}

VentanaGrafica::VentanaGrafica(int alto_px ,int ancho_px , string imagePath){
	this->alto_px = alto_px;
	this->ancho_px = ancho_px;
	this->imagePath = imagePath;
	this->window = nullptr;
	this->renderer = nullptr;
	this-> image = nullptr;

	this->inicializarSDL();
}

void VentanaGrafica::clearScenary(){
	SDL_RenderClear(this->renderer);
}

void VentanaGrafica::drawBackground(){
	//Drawing the texture
	SDL_RenderCopy(renderer, image, NULL, NULL); //Se pasa NULL para que ocupe todo el renderer
}

void VentanaGrafica::drawScenary(b2World* world){
	for(b2Body* body = world->GetBodyList(); body; body = body->GetNext()){
		this->drawStaticBody(body);
	}
}

//Dibuja un cuerpo estatico
void VentanaGrafica::drawStaticBody(b2Body* body){
	SDL_SetRenderDrawColor(this->renderer, 50, 50, 50, 255);
	int ox = ANCHO_PX/2;
	int oy = ALTO_PX/2;
	float sc = 40.0;
	//http://box2d.org/forum/viewtopic.php?f=3&t=1933
	for( b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() ){
		if( fixture->GetType() == b2Shape::e_polygon ){
			b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();

			const int count = poly->GetVertexCount();

			for( int i = 0; i < count; i++ ){
				int ind0 = (i + 1) % count ;
				b2Vec2 p0 = body->GetWorldPoint(  poly->GetVertex( ind0 ) );
				b2Vec2 p1 = body->GetWorldPoint(  poly->GetVertex(i) );

				SDL_RenderDrawLine(this->renderer, sc * p0.x + ox, -sc * p0.y + oy , sc * p1.x + ox, -sc * p1.y + oy);
			}
		}
	}
}

void VentanaGrafica::presentScenary(){
	SDL_RenderPresent(this->renderer);
}

//Aca hay que ver temas referentes a errores.
void VentanaGrafica::inicializarSDL(){
	SDL_Init(SDL_INIT_VIDEO);
	//Starting SDL2_IMAGE
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		throw;
	}
	//Opening a window
	window = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, ancho_px, alto_px, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "Error al utilizar SDL_CreateWindow() window devolvio nullptr");
		IMG_Quit();
		SDL_Quit();
		throw;
	}

	//Creating a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		SDL_DestroyWindow(window);
		logSDLError(std::cout, "Error al utilizar SDL_CreateRenderer() renderer devolvio nullptr");
		IMG_Quit();
		SDL_Quit();
		throw;
	}

	//Loading the image
	image = this->loadTexture(this->imagePath, renderer);
	if (image == nullptr){
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		throw;
	}
}

SDL_Texture* VentanaGrafica::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "Error al utilizar IMG_LoadTexture() texture devolvio nullptr");
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

