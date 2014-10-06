#include "../../headers/Vista/Drawer.h"
#include "../../headers/Vista/sprite.h"
#define COTA_INF_X -(ancho_px/5)/currentZoomFactor
#define COTA_INF_Y -(alto_px/5)/currentZoomFactor
#define COTA_SUP_X	(ancho_px/5)/currentZoomFactor
#define COTA_SUP_Y	(alto_px/5)/currentZoomFactor
#define FACTOR_DESPLAZAMIENTO 5


/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}
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
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

Drawer::Drawer(JsonParser *parser){
	this->renderer = nullptr;
	this->window = nullptr;
	this->image = nullptr;
	this->imagenPersonaje = nullptr;
	this->surfaceBackground = nullptr;

	//Utilizar parser para obtener las definciones necesarias para crear objetos
	this->ancho_px = parser->getAnchoPx();
	this->alto_px = parser->getAltoPx();
	this->alto_un = parser->getAltoUnEscenario();
	this->ancho_un = parser->getAnchoUnEscenario();
	this->imagePath = parser->getImagenFondo();
	this->currentZoomFactor = 1.0;
	this->camera = {0,0,ancho_px,alto_px};

	this->un_to_px_x = this->un_to_px_x_inicial = currentZoomFactor*(ancho_px/ancho_un);
	this->un_to_px_y = this->un_to_px_y_inicial = currentZoomFactor*(alto_px/alto_un);

	this->runWindow(ancho_px,alto_px,imagePath);
}

Drawer::~Drawer(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	SDL_Quit();
	IMG_Quit();
}

void Drawer::updateView(Escenario* model){
	this->clearScenary();
	this->drawBackground();
	this->drawScenary(model);
	this->presentScenary();
}

// ########################### //
// ##### Private methods ##### //
// ########################### //

void Drawer::clearScenary(){
	SDL_RenderClear(this->renderer);
}




void Drawer::drawBackground(){
	//este nos ayuda a terminar de acomodar las coordenadas con el zoom
	SDL_Rect clip;
	clip.x=camera.x+(currentZoomFactor-1)*(ancho_px/2);	//Coord de origen de SDL2
	clip.y=camera.y+(currentZoomFactor-1)*(alto_px/2);
	clip.w=ancho_px;
	clip.h=alto_px;

	renderTexture(image,renderer,0,0,&clip);
	//Drawing the texture
//	SDL_RenderCopy(renderer, image, NULL, NULL); //Se pasa NULL para que ocupe all el renderer
}


float coord_relativa(float referencia,float coord){
	return coord - referencia;
}

void Drawer::actualizarCamara(Personaje* personaje){

	int ox = (this->ancho_px/2)/currentZoomFactor;
	int oy = (this->alto_px/2)/currentZoomFactor;
	float pos_x = (personaje->getX())* (this->un_to_px_x)+ox;
	float pos_y = personaje->getY()* -(this->un_to_px_y)+oy;


	float x_relativa = coord_relativa(camera.x  + (camera.w/2)/currentZoomFactor,pos_x);
	float y_relativa = coord_relativa(camera.y + (camera.h/2)/currentZoomFactor,pos_y);

	float limiteIzquierdo = - ((currentZoomFactor - 1 ) * ((float)ancho_px/2));
	float limiteDerecho = (currentZoomFactor*ancho_px) - camera.w - (currentZoomFactor-1)*(ancho_px/2);
	float limiteInferior = - (currentZoomFactor-1)*alto_px/2;
	float limiteSuperior = (currentZoomFactor*alto_px) - camera.h - (currentZoomFactor-1)*(alto_px/2);

	if(x_relativa <= COTA_INF_X){
		if(camera.x > limiteIzquierdo )
			camera.x -= FACTOR_DESPLAZAMIENTO;
	}
	else{
		if(x_relativa >= COTA_SUP_X){
			if( camera.x < limiteDerecho )
				camera.x+=FACTOR_DESPLAZAMIENTO;
		}
	}
	if(y_relativa <= COTA_INF_Y){
		if(camera.y > limiteInferior )
			camera.y -= FACTOR_DESPLAZAMIENTO;
	}
	else if(y_relativa >= COTA_SUP_Y){
		if(camera.y < limiteSuperior )
			camera.y += FACTOR_DESPLAZAMIENTO;
	}
}

void Drawer::drawScenary(Escenario* model){
	std::list<Figura*>* figuras = model->getFiguras();
	actualizarCamara(model->getPersonaje());
	for (auto figura : *figuras){
		this->drawFigura(figura);
	}
	this->drawCharacter(model->getPersonaje());
}

//Convierte un color representado a partir de una cadena de caracteres a su valor numerico red green blue.
//la cadena debe ser del tipo "#02FF12"
char* convertir_hex_a_rgb (std::string color){

        char* resultado = new char[3];

        const char *red = (color.substr(1,2)).c_str();
        resultado[0] = strtol(red,NULL,16);

        const char *green = (color.substr(3,2)).c_str();
        resultado[1] =strtol(green,NULL,16);

        const char *blue = (color.substr(5,2)).c_str();
        resultado[2] =strtol(blue,NULL,16);

        return resultado;
}



//Dibuja una figura
void Drawer::drawFigura(Figura* figura){
	//NOTA: cambiar esto por la forma actual. Vamos a tener que tener en cuenta a los circulos.
	char* rgb = convertir_hex_a_rgb(figura->getColor());
	int ox = this->ancho_px/2;
	int oy = this->alto_px/2;

	for( b2Fixture *fixture = figura->GetFixtureList(); fixture; fixture = fixture->GetNext() ){
		if( fixture->GetType() == b2Shape::e_polygon ){
			b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();
			const int count = poly->GetVertexCount();
			Sint16* xCoordOfVerts = new Sint16 [count+1];
			Sint16* yCoordOfVerts = new Sint16 [count+1];

			for(int i = 0; i < count ; i++){
				b2Vec2 p1 = figura->GetWorldPoint(poly->GetVertex(i));
				xCoordOfVerts[i] = (Sint16)((un_to_px_x) * (p1.x) + ox);
				yCoordOfVerts[i] = (Sint16)(-un_to_px_y * p1.y + oy);

				xCoordOfVerts[i] = coord_relativa(camera.x,xCoordOfVerts[i]);
				yCoordOfVerts[i] = coord_relativa(camera.y,yCoordOfVerts[i]);
			}

			filledPolygonRGBA(this->renderer, xCoordOfVerts, yCoordOfVerts, count, rgb[0], rgb[1], rgb[2], 255);

			delete [] xCoordOfVerts;
			delete [] yCoordOfVerts;
		}
        else if( fixture->GetType() == b2Shape::e_circle ){
			Circulo* circ = (Circulo*) figura;
			int centro_x=circ->getCoordX() * un_to_px_x + ox;
			int centro_y =circ->getCoordY() * -un_to_px_y + oy;

			//Opcion de dibujar el radio nomá
			//Esta linea va a ser el radio que va a ir girando con el circulo
			int borde_x = ( circ->getRadio()* cos(circ->getAngulo()) * un_to_px_x) + centro_x;
			int borde_y = centro_y - ( circ->getRadio()* sin(circ->getAngulo()) * un_to_px_y);


			//cambiando coordenadas por las relativas a la camara
			centro_x = coord_relativa(camera.x,centro_x);
			centro_y = coord_relativa(camera.y,centro_y);

			filledEllipseRGBA(this->renderer, centro_x, centro_y, circ->getRadio() * un_to_px_x, circ->getRadio() * un_to_px_y,
							  rgb[0], rgb[1], rgb[2], 255);


			//cambiando coordenadas por las relativas a la camara
			borde_x = coord_relativa(camera.x,borde_x);
			borde_y = coord_relativa(camera.y,borde_y);

			lineRGBA(renderer,centro_x,centro_y,borde_x,borde_y,(rgb[0]+120)%255, (rgb[1]+120)%255,(rgb[2]+120)%255, 255);
        }
	}
	delete [] rgb;
}



int anchoPersonaje(float un_to_px_x){
	return (MITAD_ANCHO_PERSONAJE*2) * un_to_px_x + 15 ;
}
int altoPersonaje(float un_to_px_y){
	return ((MITAD_ALTO_PERSONAJE * 2) * un_to_px_y);
}

void Drawer::drawCharacter(Personaje* person){
		int ox = this->ancho_px/2;
		int oy = this->alto_px/2;
		char codigo_estado = person->state->getCode();
		char orientacion = person->getOrientacion();
		SDL_Texture *textura = this->imagenPersonaje;
		float pos_x = (person->getX())* (this->un_to_px_x)+ox;
		float pos_y = person->getY()* -(this->un_to_px_y)+oy;
		pos_x = coord_relativa(camera.x,pos_x);
		pos_y = coord_relativa(camera.y,pos_y);

		switch(codigo_estado){
			case JUMPING:
				drawPersonajeSaltando(this->renderer,textura,orientacion,pos_x,pos_y,anchoPersonaje(un_to_px_x),altoPersonaje(un_to_px_y));
				break;
			case STANDBY:
				drawPersonajeStandBy(renderer,textura,orientacion,pos_x,pos_y,anchoPersonaje(un_to_px_x),altoPersonaje(un_to_px_y));
				break;
			case WALKING:
				drawPersonajeCaminando(renderer,textura,orientacion,pos_x,pos_y,anchoPersonaje(un_to_px_x),altoPersonaje(un_to_px_y));
				break;
			case FALLING:
				drawPersonajeCayendo(renderer,textura,orientacion,pos_x,pos_y,anchoPersonaje(un_to_px_x),altoPersonaje(un_to_px_y));
				break;
		}


}

void Drawer::presentScenary(){
	SDL_RenderPresent(this->renderer);
}

void Drawer::runWindow(int ancho_px ,int alto_px ,string imagePath){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		logSDLError( "Error al inicializar SDL2. Verifique la instalacion de la libreria");
        throw SDLError();
    }
	//Starting SDL2_IMAGE
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		manageSDL2_imageError();
	}

	//Opening a window
	window = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, ancho_px, alto_px, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		manageCreateWindowError();
	}

	//Creating a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		manageCreateRendererError();
	}

	//Loading the image
	image = this->loadTexture(this->imagePath,renderer);
	if (image == nullptr){
		manageLoadBackgroundError();
	}
	surfaceBackground = IMG_Load(this->imagePath.c_str()); //Si falla la carga de la imagen, se contempla en el manageLoadBackgroundError

	imagenPersonaje = this->loadTexture(SPRITE_PATH,this->renderer);
	if (imagenPersonaje == nullptr){
		manageLoadCharacterError();
	}
}

void Drawer::manageSDL2_imageError(){
	logSDLError( "Error al inicializar SDL2_image");
	SDL_Quit();
	throw SDLError();
}

void Drawer::manageCreateWindowError(){
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al utilizar SDL_CreateWindow() window devolvio nullptr");
	throw SDLError();
}

void Drawer::manageCreateRendererError(){
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al utilizar SDL_CreateRenderer() renderer devolvio nullptr");
	throw SDLError();
}

void Drawer::manageLoadBackgroundError(){
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
	throw SDLError();
}

void Drawer::manageLoadCharacterError(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
	throw SDLError();
}

SDL_Texture* Drawer::loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	return texture;
}
void Drawer::logSDLError(const std::string &msg){
	Log::instance()->append(msg + SDL_GetError(),Log::ERROR);
}

void Drawer::zoomIn(){
	currentZoomFactor += factor;
	//Zoom in a la imagen de fondo
	SDL_Surface* surfaceTemp = rotozoomSurface(surfaceBackground,rotation,currentZoomFactor,smoothing_off);
	//Libero memoria del texture actual que se va a reemplazar
	SDL_DestroyTexture(image);
	image = SDL_CreateTextureFromSurface(renderer,surfaceTemp);
	//Libero memoria del surface temporal
	SDL_FreeSurface(surfaceTemp);

	//Zoom in a la escala de las figuras
	un_to_px_x = un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y = un_to_px_y_inicial * currentZoomFactor;
}

void Drawer::zoomOut(){
	currentZoomFactor -= factor;
	//Zoom out a la imagen de fondo
	SDL_Surface* surfaceTemp = rotozoomSurface(surfaceBackground,rotation,currentZoomFactor,smoothing_off);
	//Libero memoria del texture actual que se va a reemplazar
	SDL_DestroyTexture(image);
	image = SDL_CreateTextureFromSurface(renderer,surfaceTemp);
	//Libero memoria del surface temporal
	SDL_FreeSurface(surfaceTemp);

	//Zoom out a la escala de las figuras
	un_to_px_x =un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y =un_to_px_y_inicial * currentZoomFactor;
}
