#include "../../headers/Vista/Drawer.h"
#include "../../headers/Vista/sprite.h"
/*
#define COTA_INF_X -(ancho_px - (ancho_px*(currentZoomFactor-1)))/5
#define COTA_INF_Y -(alto_px-(alto_px)*(currentZoomFactor-1))/5
#define COTA_SUP_X	(ancho_px-ancho_px*(currentZoomFactor-1))/5
#define COTA_SUP_Y	(alto_px-(alto_px)*(currentZoomFactor-1))/5
*/


#define FACTOR_DESPLAZAMIENTO 6
#define FACTOR_CONVERSION_UN_A_PX 32
#define COTA_INF_X -(ancho_px )/5
#define COTA_INF_Y -(alto_px)/5
#define COTA_SUP_X	(ancho_px)/5
#define COTA_SUP_Y	(alto_px)/5


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
		dst.w = 800;
		dst.h = 600;
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
	this->messageAboutLifes = nullptr;
	this->messageAboutPoints = nullptr;
	this->fontToBeUsed = nullptr;
	this->fontPath = "resources/leadcoat.ttf";

	//Utilizar parser para obtener las definciones necesarias para crear objetos
	this->ancho_px = parser->getAnchoPx();
	this->alto_px = parser->getAltoPx();
	this->alto_un = parser->getAltoUnEscenario();
	this->ancho_un = parser->getAnchoUnEscenario();
	this->imagePath = parser->getImagenFondo();
	this->currentZoomFactor = 1.0;
	this->camera = {0,0,ancho_px,alto_px};
	this->coordRel ={0,0,ancho_px,alto_px};

	this->un_to_px_x = this->un_to_px_x_inicial = currentZoomFactor*FACTOR_CONVERSION_UN_A_PX;
	this->un_to_px_y = this->un_to_px_y_inicial = currentZoomFactor*FACTOR_CONVERSION_UN_A_PX;

	this->runWindow(ancho_px,alto_px,imagePath);
}

Drawer::~Drawer(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	SDL_FreeSurface(messageAboutLifes);
	SDL_FreeSurface(messageAboutPoints);
	TTF_CloseFont(fontToBeUsed);
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

void Drawer::updateView(Escenario* model){
	this->clearScenary();
	this->drawBackground();
	this->drawScenary(model);
	this->drawMessages();
	this->presentScenary();
}

// ########################### //
// ##### Private methods ##### //
// ########################### //

void Drawer::clearScenary(){
	SDL_RenderClear(this->renderer);
}

void Drawer::drawBackground(){
	renderTexture(image,renderer,0,0,&camera);
}

void Drawer::drawMessages(){
	SDL_Color textColor = { 255, 255, 255 };
	this->messageAboutPoints = TTF_RenderText_Solid(fontToBeUsed,"1000 Puntos",textColor);
	this->messageAboutLifes = TTF_RenderText_Solid(fontToBeUsed,"3 vidas ", textColor);

	if ((messageAboutPoints == nullptr) || (messageAboutLifes == nullptr))
		manageDrawMessagesError();

	//Set the coordinates which we want to draw to
	float coordXDelMensaje = ancho_px/2 - 1 * un_to_px_x_inicial;
	float coordYDelMensaje = alto_px/2 - 8 * un_to_px_y_inicial;

	//Render the first message
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,messageAboutPoints);
	renderTexture(texture,renderer,coordXDelMensaje,coordYDelMensaje);

	//Render the other message
	texture = SDL_CreateTextureFromSurface(renderer,messageAboutLifes);
	coordXDelMensaje += 10 * un_to_px_x_inicial;
	renderTexture(texture,renderer,coordXDelMensaje,coordYDelMensaje);
	SDL_DestroyTexture(texture);
}

float coord_relativa(float referencia,float coord){
	return coord - referencia;
}

void ajusteFueraDeLimite(SDL_Rect &rect,int limIzq,int limDer,int limInf,int limSup){
		if(rect.y < limInf)
			rect.y = limInf;
		if(rect.y > limSup)
			rect.y = limSup;
		if(rect.x < limIzq)
			rect.x = limIzq;
		if(rect.x > limDer)
			rect.x = limDer;
}

void Drawer::actualizarCamara(Personaje* personaje){
	float ancho_imagen = ancho_un *FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un *FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen/2) ;
	int oy = (alto_imagen/2);

	float pos_x = (personaje->getX())* (this->un_to_px_x)  +ox;
	float pos_y = personaje->getY()* -(this->un_to_px_y)+oy;

	float x_relativa = coord_relativa(coordRel.x  + (coordRel.w/2),pos_x);
	float y_relativa = coord_relativa(coordRel.y + (coordRel.h/2),pos_y);

	int limIzqCamera =0;
	int limDerCamera = ancho_imagen  - camera.w;
	int limInfCamera = 0;
	int limSupCamera = alto_imagen - camera.h;

	int limiteIzquierdo = - (currentZoomFactor - 1 ) * ancho_imagen/2;
	int limiteDerecho = ancho_imagen  - coordRel.w  + (currentZoomFactor-1)*(ancho_imagen/2);
	int limiteInferior  = - ((currentZoomFactor-1) * alto_imagen/2);
	int limiteSuperior = alto_imagen - coordRel.h + (currentZoomFactor-1)*(alto_imagen/2);

	if(x_relativa <= COTA_INF_X){
		if(camera.x > limIzqCamera )
			camera.x -= FACTOR_DESPLAZAMIENTO/currentZoomFactor;

		if(coordRel.x > limiteIzquierdo )
			coordRel.x -= FACTOR_DESPLAZAMIENTO;
	}
	else{
		if(x_relativa >= COTA_SUP_X){
			if( camera.x < limDerCamera )
				camera.x+=FACTOR_DESPLAZAMIENTO/currentZoomFactor;

			if( coordRel.x < limiteDerecho )
				coordRel.x+=FACTOR_DESPLAZAMIENTO;
		}
	}
	if(y_relativa <= COTA_INF_Y){
		if(camera.y > limInfCamera )
			camera.y -= FACTOR_DESPLAZAMIENTO/currentZoomFactor;

		if(coordRel.y > limiteInferior )
			coordRel.y -= FACTOR_DESPLAZAMIENTO;
	}
	else if(y_relativa >= COTA_SUP_Y){
		if(camera.y < limSupCamera )
			camera.y += FACTOR_DESPLAZAMIENTO/currentZoomFactor;

		if(coordRel.y < limiteSuperior )
			coordRel.y += FACTOR_DESPLAZAMIENTO;
	}

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera, limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho, limiteInferior, limiteSuperior);
}

void Drawer::drawScenary(Escenario* model){
	std::list<Figura*>* figuras = model->getFiguras();
	//actualizarFondo(model->getPersonaje());
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
	int ox = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX) /2;
	int oy = (this->alto_un *FACTOR_CONVERSION_UN_A_PX) /2;

	for( b2Fixture *fixture = figura->GetFixtureList(); fixture; fixture = fixture->GetNext() ){
		if( fixture->GetType() == b2Shape::e_polygon ){
			b2PolygonShape *poly = (b2PolygonShape*)fixture->GetShape();
			const int count = poly->GetVertexCount();
			Sint16* xCoordOfVerts = new Sint16 [count+1];
			Sint16* yCoordOfVerts = new Sint16 [count+1];

			for(int i = 0; i < count ; i++){
				b2Vec2 p1 = figura->GetWorldPoint(poly->GetVertex(i));
				xCoordOfVerts[i] = (Sint16)((un_to_px_x) * (p1.x) + ox);
				yCoordOfVerts[i] = (Sint16)(-un_to_px_y * p1.y +oy);

				xCoordOfVerts[i] = (Sint16)coord_relativa(coordRel.x,xCoordOfVerts[i]);
				yCoordOfVerts[i] = (Sint16)coord_relativa(coordRel.y,yCoordOfVerts[i]);
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
			centro_x = coord_relativa(coordRel.x,centro_x);
			centro_y = coord_relativa(coordRel.y,centro_y);

			filledEllipseRGBA(this->renderer, centro_x, centro_y, circ->getRadio() * un_to_px_x, circ->getRadio() * un_to_px_y,
							  rgb[0], rgb[1], rgb[2], 255);


			//cambiando coordenadas por las relativas a la camara
			borde_x = coord_relativa(coordRel.x,borde_x);
			borde_y = coord_relativa(coordRel.y,borde_y);

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
		int ox = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX) /2;
		int oy = (this->alto_un *FACTOR_CONVERSION_UN_A_PX) /2;
		char codigo_estado = person->state->getCode();
		char orientacion = person->getOrientacion();
		SDL_Texture *textura = this->imagenPersonaje;
		float pos_x = (person->getX())* (this->un_to_px_x)+ox;
		float pos_y = (person->getY())* -(this->un_to_px_y)+oy;
		pos_x = coord_relativa(coordRel.x,pos_x);
		pos_y = coord_relativa(coordRel.y,pos_y);

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

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
	    manageSDL_ttfError();
	}

	int sizeOfTheFont = 20;
	fontToBeUsed = TTF_OpenFont(this->fontPath.c_str(),sizeOfTheFont);
	if (fontToBeUsed == nullptr){
		manageSDL_ttfLoadFontError();
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
void Drawer::manageSDL_ttfError(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al inicializar SDL_ttf");
	throw SDLError();
}

void Drawer::manageSDL_ttfLoadFontError(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error al cargar la fuente de SDL_ttf");
	throw SDLError();
}

void Drawer::manageDrawMessagesError(){
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(surfaceBackground);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	logSDLError( "Error de SDL_ttf al cargar escribir mensajes en la pantalla");
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
	camera.h = alto_px/currentZoomFactor;
	camera.w = ancho_px/currentZoomFactor;

	//Zoom in a la escala de las figuras
	un_to_px_x = un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y = un_to_px_y_inicial * currentZoomFactor;
}

void Drawer::zoomOut(){
	currentZoomFactor -= factor;
	int ancho_anterior= camera.w;
	int alto_anterior = camera.h;
	camera.h = alto_px/currentZoomFactor;
	camera.w = ancho_px/currentZoomFactor;

	//Me fijo que no se expanda mas de lo que deberia
	int proximo_x_maximo = ancho_un* FACTOR_CONVERSION_UN_A_PX - 2*camera.w + ancho_anterior ;
	int proximo_y_maximo = alto_un* FACTOR_CONVERSION_UN_A_PX - 2*camera.h + alto_anterior ;

	if(proximo_x_maximo <= 0 || proximo_y_maximo <= 0 ){
		currentZoomFactor += factor;
		return;
	}

	if(camera.x >= proximo_x_maximo ){
		camera.x = proximo_x_maximo;

	}
	if(camera.y >= proximo_y_maximo){
		camera.y = proximo_y_maximo;

	}

	//Zoom out a la escala de las figuras

	un_to_px_x =un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y =un_to_px_y_inicial * currentZoomFactor;
}
