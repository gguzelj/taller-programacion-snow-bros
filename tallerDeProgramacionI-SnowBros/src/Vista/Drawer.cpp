#include "../../headers/Vista/Drawer.h"

Drawer::Drawer(JsonParser *parser){
	this->renderer = nullptr;
	this->window = nullptr;
	this->image = nullptr;

	//Utilizar parser para obtener las definciones necesarias para crear objetos
	this->ancho_px = parser->getAnchoPx();
	this->alto_px = parser->getAltoPx();
	this->imagePath = "resources/" + parser->getImagenFondo();

	cout<<"Ancho: "<<ancho_px<<" Alto: "<<alto_px<<" imagePath: "<<imagePath<<endl;

	cout<<"tipo obj1: "<< parser->getTipoObjeto(0)<<endl;
	cout<<"Coordenada x Obj1: "<< parser->getCoorXObjeto(0) <<endl;
	cout<<"Coordenada y Obj1: "<< parser->getCoorYObjeto(0) <<endl;
	cout<<"ancho obj1: "<< parser->getAnchoObjeto(0) <<endl;
	cout<<"alto obj1: "<< parser->getAltoOBjeto(0) <<endl;
	cout<<"Color Obj1: "<< parser->getColorObjeto(0) <<endl;
	cout<<"Rotacion Obj1: "<< parser->getRotObjeto(0) <<endl;
	cout<<"masa Obj1: "<< parser->getMasaObjeto(0) <<endl;
	cout<<"obj 1 estatico? "<< parser->esObjetoEstatico(0) <<endl;

	cout<<"Cantidad de objetos en archivo? "<< parser->getCantidadObjetos()<<endl;

	this->runWindow(ancho_px,alto_px,imagePath);
}

Drawer::~Drawer(){
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();
}

void Drawer::updateView(Escenario* model){
	this->clearScenary();
	this->drawBackground();
//	this->drawScenary(model);
	this->presentScenary();
	SDL_Delay(100);
}

// ########################### //
// ##### Private methods ##### //
// ########################### //

void Drawer::clearScenary(){
	SDL_RenderClear(this->renderer);
}

void Drawer::drawBackground(){
	//Drawing the texture
	SDL_RenderCopy(renderer, image, NULL, NULL); //Se pasa NULL para que ocupe todo el renderer
}

void Drawer::drawScenary(Escenario* model){
	std::list<Figura>* figuras = model->getFiguras();
	for (auto figura : *figuras){
		this->drawFigura(&figura);
	}
	this->drawCharacter();
}

//Dibuja una figura
void Drawer::drawFigura(Figura* figura){
	//NOTA: cambiar esto por la forma actual. Vamos a tener que tener en cuenta a los circulos.
	SDL_SetRenderDrawColor(this->renderer, 50, 50, 50, 255); //Color que se va a dibujar el contorno. Esto hay que cambiarlo por el color del objeto
	int ox = this->ancho_px/2;
	int oy = this->alto_px/2;
	float sc = 40.0;

	for( b2Fixture *fixture = figura->GetFixtureList(); fixture; fixture = fixture->GetNext() ){
		if( fixture->GetType() == b2Shape::e_polygon ){
			b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();

			const int count = poly->GetVertexCount();

			for( int i = 0; i < count; i++ ){
				int ind0 = (i + 1) % count ;
				b2Vec2 p0 = figura->GetWorldPoint(  poly->GetVertex( ind0 ) );
				b2Vec2 p1 = figura->GetWorldPoint(  poly->GetVertex(i) );

				SDL_RenderDrawLine(this->renderer, sc * p0.x + ox, -sc * p0.y + oy , sc * p1.x + ox, -sc * p1.y + oy);
			}
		}
//		else if( fixture->GetType() == b2Shape::e_circle ){
//			 b2CircleShape* circleShape = (b2CircleShape*)fixture->GetShape();
//		}
	}
}

void Drawer::drawCharacter(){}

void Drawer::presentScenary(){
	SDL_RenderPresent(this->renderer);
}

void Drawer::runWindow(int ancho_px ,int alto_px ,string imagePath){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw;
    }
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


SDL_Texture* Drawer::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "Error al utilizar IMG_LoadTexture() texture devolvio nullptr");
	}
	return texture;
}
void Drawer::logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

