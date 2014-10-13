#include "../../headers/Vista/Drawer.h"
#include "../../headers/Vista/sprite.h"

#define FACTOR_DESPLAZAMIENTO 5 *currentZoomFactor
#define FACTOR_CONVERSION_UN_A_PX 32
#define COTA_INF_X -(ancho_px )/5
#define COTA_INF_Y -(alto_px)/5
#define COTA_SUP_X	(ancho_px)/5
#define COTA_SUP_Y	(alto_px)/5

void Drawer::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
		SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void Drawer::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
		SDL_Rect *clip = nullptr) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = ancho_px;
		dst.h = alto_px;
	} else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

//Cargar aca las imagenes de las figuras que faltan.
bool Drawer::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load images
	if(!rectangleTexture.loadFromFile(rectangleImage, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!circleTexture.loadFromFile(circleImage, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!triangleTexture.loadFromFile(triangleImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!squareTexture.loadFromFile(squareImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!pentagonTexture.loadFromFile(pentagonImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!hexagonTexture.loadFromFile(hexagonImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!trapexTexture.loadFromFile(trapexImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}
	if(!paralelogramTexture.loadFromFile(paralelogramImagePath, renderer))
	{
		printf( "Failed to load fondito texture!\n" );
		success = false;
	}

	return success;
}

Drawer::Drawer(JsonParser *parser) {
	this->renderer = nullptr;
	this->window = nullptr;
	this->image = nullptr;
	this->imagenPersonaje = nullptr;
	this->messageAboutLifes = nullptr;
	this->messageAboutPoints = nullptr;
	this->fontToBeUsed = nullptr;

	//Paths
	this->imagePath = parser->getImagenFondo();
	this->fontPath = "resources/dailypla.ttf";
	this->rectangleImage = "resources/textures/rectangle.png";
	this->circleImage = "resources/textures/circle.png";
	this->triangleImagePath = "resources/textures/triangle.png";
	this->squareImagePath = "resources/textures/cuadrado.png";
	this->pentagonImagePath = "resources/textures/pentagon.png";
	this->hexagonImagePath = "resources/textures/hexagon.png";
	this->trapexImagePath = "resources/textures/trapecio.png";
	this->paralelogramImagePath = "resources/textures/paralelogramo.png";

	this->ancho_px = parser->getAnchoPx();
	this->alto_px = parser->getAltoPx();
	this->alto_un = parser->getAltoUnEscenario();
	this->ancho_un = parser->getAnchoUnEscenario();
	this->currentZoomFactor = 1.0;
	this->camera = {0,0,ancho_px,alto_px};
	this->coordRel = {0,0,ancho_px,alto_px};

	this->un_to_px_x = this->un_to_px_x_inicial = currentZoomFactor
			* FACTOR_CONVERSION_UN_A_PX;
	this->un_to_px_y = this->un_to_px_y_inicial = currentZoomFactor
			* FACTOR_CONVERSION_UN_A_PX;

	this->runWindow(ancho_px, alto_px, imagePath);
}

Drawer::~Drawer() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_FreeSurface(messageAboutLifes);
	SDL_FreeSurface(messageAboutPoints);
	TTF_CloseFont(fontToBeUsed);
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

void Drawer::updateView(Escenario* model) {
	this->clearScenary();
	this->drawBackground();
	this->drawScenary(model);
	this->drawMessages();
	this->presentScenary();
}


// ############################### //
// ##### Auxiliray functions ##### //
// ############################### //


float coord_relativa(float referencia, float coord) {
	return coord - referencia;
}

//Convierte un color representado a partir de una cadena de caracteres a su valor numerico red green blue.
//la cadena debe ser del tipo "#02FF12"
char* convertir_hex_a_rgb(std::string color) {

	char* resultado = new char[3];

	const char *red = (color.substr(1, 2)).c_str();
	resultado[0] = strtol(red, NULL, 16);

	const char *green = (color.substr(3, 2)).c_str();
	resultado[1] = strtol(green, NULL, 16);

	const char *blue = (color.substr(5, 2)).c_str();
	resultado[2] = strtol(blue, NULL, 16);

	return resultado;
}

int anchoPersonaje(float un_to_px_x) {
	return (MITAD_ANCHO_PERSONAJE * 2) * un_to_px_x + 15;
}
int altoPersonaje(float un_to_px_y) {
	return ((MITAD_ALTO_PERSONAJE * 2) * un_to_px_y);
}

// ########################### //
// ##### Private methods ##### //
// ########################### //
void Drawer::clearScenary() {
	SDL_RenderClear(this->renderer);
}

void Drawer::drawBackground() {
	renderTexture(image, renderer, 0, 0, &camera);
}

void Drawer::drawScenary(Escenario* model) {
	std::list<Figura*>* figuras = model->getFiguras();
	actualizarCamara(model->getPersonaje());
	for (auto figura : *figuras) {
		this->drawFigura(figura);
	}
	this->drawCharacter(model->getPersonaje());
}

//Dibuja una figura
void Drawer::drawFigura(Figura* figura) {
	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	b2Vec2 p = figura->GetCenter();

	if (figura->type == "rectangulo"){
	    Rectangulo* rect = static_cast<Rectangulo *> (figura);

		float ancho = rect->getAncho();
		float alto = rect->getAlto();

		rectangleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-ancho/2) + ox),
										  coord_relativa(coordRel.y, -un_to_px_y * (p.y+alto/2) + oy),
										  ancho*un_to_px_x, alto*un_to_px_y,
										  nullptr, rect->getAngulo()*-RADTODEG, nullptr);
	}

	if (figura->type == "circulo"){
	    Circulo* circ = static_cast<Circulo *> (figura);
	    float radio = circ->getRadio();

	    circleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-radio) + ox),
				 					   coord_relativa(coordRel.y, -un_to_px_y * (p.y+radio) + oy),
				 					   2*radio*un_to_px_x, 2*radio*un_to_px_y,
				 					   nullptr, circ->getAngulo()*-RADTODEG, nullptr);
	}

	if (figura->type == "poligono"){
		Poligono* polygon = static_cast<Poligono *> (figura);
		float escala = polygon->getEscala();
		int lados = polygon->getLados();

		if(lados == 3){
			triangleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-escala) + ox),
					   coord_relativa(coordRel.y, -un_to_px_y * (p.y+0.9*escala) + oy),
					   2*escala*un_to_px_x, escala*2*un_to_px_y,
					   nullptr, polygon->getAngulo()*-RADTODEG, nullptr);
		}
		else if (lados == 4){
			squareTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-escala) + ox),
											  coord_relativa(coordRel.y, -un_to_px_y * (p.y+escala) + oy),
											  2*escala*un_to_px_x, 2*escala*un_to_px_x,
											  nullptr, polygon->getAngulo()*-RADTODEG, nullptr);
		}
		else if (lados == 5){
			pentagonTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-escala*0.951057) + ox),
					   coord_relativa(coordRel.y, -un_to_px_y * (p.y+escala*(1.809017/2)) + oy),
					   2*escala*0.951057*un_to_px_x, escala*1.809017*un_to_px_y,
					   nullptr, polygon->getAngulo()*-RADTODEG, nullptr);
		}
		else{		// SI no entro en ningun if anterior, es un hexagono
			hexagonTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-escala*sin(2*M_PI/6)) + ox),
					   coord_relativa(coordRel.y, -un_to_px_y * (p.y+escala) + oy),
					   2*escala*sin(2*M_PI/6)*un_to_px_x, 2*escala*un_to_px_y,
					   nullptr, polygon->getAngulo()*-RADTODEG, nullptr);
		}
	}
	//TODO arreglar como se muestra la imagen de trapecio
	if(figura->type == "trapecio"){
	    Trapecio* trap = static_cast<Trapecio *> (figura);

		float ancho = trap->getBaseMayor();
		float mediana = (ancho+ancho*0.68)/2;
		float alto = trap->getAlto();

		trapexTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-ancho/2) + ox),
										  coord_relativa(coordRel.y, -un_to_px_y * (p.y+alto/2) + oy),
										  ancho*un_to_px_x, alto*un_to_px_y,
										  nullptr, trap->getAngulo()*-RADTODEG, nullptr);
	}
	//TODO arreglar como se muestra la imagen de paralelogramo
	if(figura->type == "paralelogramo"){
	    Paralelogramo* paralelogramo = static_cast<Paralelogramo *> (figura);

		float ancho = paralelogramo->getAncho();
		float alto = paralelogramo->getAlto();

		paralelogramTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (p.x-ancho*5/9) + ox),
										  coord_relativa(coordRel.y, -un_to_px_y * (p.y+alto/2) + oy),
										  ancho*1.2*un_to_px_x, alto*un_to_px_y,
										  nullptr, paralelogramo->getAngulo()*-RADTODEG, nullptr);
	}

	char* rgb = convertir_hex_a_rgb(figura->getColor());
    //Old method lo deje para que se vean los dos por ahora. Luego borrar esto.
    for (b2Fixture *fixture = figura->GetFixtureList(); fixture; fixture =
                    fixture->GetNext()) {
            if (fixture->GetType() == b2Shape::e_polygon) {
                    b2PolygonShape *poly = (b2PolygonShape*) fixture->GetShape();
                    const int count = poly->GetVertexCount();
                    Sint16* xCoordOfVerts = new Sint16[count + 1];
                    Sint16* yCoordOfVerts = new Sint16[count + 1];

                    for (int i = 0; i < count; i++) {
                            b2Vec2 p1 = figura->GetWorldPoint(poly->GetVertex(i));
                            xCoordOfVerts[i] = (Sint16) ((un_to_px_x) * (p1.x) + ox);
                            yCoordOfVerts[i] = (Sint16) (-un_to_px_y * p1.y + oy);

                            xCoordOfVerts[i] = (Sint16) coord_relativa(coordRel.x, xCoordOfVerts[i]);
                            yCoordOfVerts[i] = (Sint16) coord_relativa(coordRel.y, yCoordOfVerts[i]);
                    }
                    polygonRGBA(renderer, xCoordOfVerts, yCoordOfVerts, count, rgb[0], rgb[1], rgb[2], 255);
                    //texturedPolygon(renderer,xCoordOfVerts,yCoordOfVerts,count,imagePolygon,xCoordOfVerts[0],yCoordOfVerts[0]);

                    delete[] xCoordOfVerts;
                    delete[] yCoordOfVerts;

            } else if (fixture->GetType() == b2Shape::e_circle) {
                    Circulo* circ = (Circulo*) figura;
                    int centro_x = circ->getCoordX() * un_to_px_x + ox;
                    int centro_y = circ->getCoordY() * -un_to_px_y + oy;

                    //Opcion de dibujar el radio nomá
                    //Esta linea va a ser el radio que va a ir girando con el circulo
                    int borde_x = (circ->getRadio() * cos(circ->getAngulo())
                                    * un_to_px_x) + centro_x;
                    int borde_y = centro_y
                                    - (circ->getRadio() * sin(circ->getAngulo()) * un_to_px_y);

                    //cambiando coordenadas por las relativas a la camara
                    centro_x = coord_relativa(coordRel.x, centro_x);
                    centro_y = coord_relativa(coordRel.y, centro_y);

                    ellipseRGBA(this->renderer, centro_x, centro_y,
                                    circ->getRadio() * un_to_px_x,
                                    circ->getRadio() * un_to_px_y, rgb[0], rgb[1], rgb[2], 255);

                    //cambiando coordenadas por las relativas a la camara
                    borde_x = coord_relativa(coordRel.x, borde_x);
                    borde_y = coord_relativa(coordRel.y, borde_y);

                    lineRGBA(renderer, centro_x, centro_y, borde_x, borde_y,
                                    (rgb[0] + 120) % 255, (rgb[1] + 120) % 255,
                                    (rgb[2] + 120) % 255, 255);
            }
    }
    delete[] rgb;
}

void Drawer::drawCharacter(Personaje* person) {
	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	char codigo_estado = person->state->getCode();
	char orientacion = person->getOrientacion();
	SDL_Texture *textura = this->imagenPersonaje;
	float pos_x = (person->getX()) * (this->un_to_px_x) + ox;
	float pos_y = (person->getY()) * -(this->un_to_px_y) + oy;
	pos_x = coord_relativa(coordRel.x, pos_x);
	pos_y = coord_relativa(coordRel.y, pos_y);

	switch (codigo_estado) {
	case JUMPING:
		drawPersonajeSaltando(this->renderer, textura, orientacion, pos_x,
				pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case STANDBY:
		drawPersonajeStandBy(renderer, textura, orientacion, pos_x, pos_y,
				anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case WALKING:
		drawPersonajeCaminando(renderer, textura, orientacion, pos_x, pos_y,
				anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case FALLING:
		drawPersonajeCayendo(renderer, textura, orientacion, pos_x, pos_y,
				anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	}
}

void Drawer::drawMessages() {
	SDL_Color textColor = { 255, 255, 255 };
	this->messageAboutPoints = TTF_RenderText_Solid(fontToBeUsed, "1000 Puntos",
			textColor);
	this->messageAboutLifes = TTF_RenderText_Solid(fontToBeUsed, "3 vidas ",
			textColor);

	if ((messageAboutPoints == nullptr) || (messageAboutLifes == nullptr))
		manageDrawMessagesError();

	//Set the coordinates which we want to draw to
	float coordXDelMensaje = ancho_px / 2 - 1 * un_to_px_x_inicial; //Centro en x
	float coordYDelMensaje = 1 * un_to_px_y_inicial; //Parte superior de la pantalla

	//Render the first message
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,
			messageAboutPoints);
	renderTexture(texture, renderer, coordXDelMensaje, coordYDelMensaje);

	//Render the other message
	texture = SDL_CreateTextureFromSurface(renderer, messageAboutLifes);
	coordXDelMensaje += 10 * un_to_px_x_inicial;
	renderTexture(texture, renderer, coordXDelMensaje, coordYDelMensaje);
	SDL_DestroyTexture(texture);
}

void Drawer::presentScenary() {
	SDL_RenderPresent(this->renderer);
}

void ajusteFueraDeLimite(SDL_Rect &rect, int limIzq, int limDer, int limInf,
		int limSup) {
	if (rect.y < limInf)
		rect.y = limInf;
	if (rect.y > limSup)
		rect.y = limSup;
	if (rect.x < limIzq)
		rect.x = limIzq;
	if (rect.x > limDer)
		rect.x = limDer;
}

void Drawer::inicializarCamara(Personaje* personaje) {
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	float pos_x = (personaje->getX()) * (this->un_to_px_x) + ox;
	float pos_y = personaje->getY() * -(this->un_to_px_y) + oy;

	camera.x = pos_x - camera.w / 2;
	camera.y = pos_y - camera.h / 2;

	coordRel.x = camera.x;
	coordRel.y = camera.y;

	int limIzqCamera = 0;
	int limDerCamera = ancho_imagen - camera.w;
	int limInfCamera = 0;
	int limSupCamera = alto_imagen - camera.h;

	int limiteIzquierdo = 0;
	int limiteDerecho = ancho_imagen - coordRel.w
			+ (currentZoomFactor - 1) * (ancho_imagen);
	int limiteInferior = 0;
	int limiteSuperior = alto_imagen - coordRel.h
			+ (currentZoomFactor - 1) * (alto_imagen);

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera,
			limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho,
			limiteInferior, limiteSuperior);

}

void Drawer::actualizarCamara(Personaje* personaje) {
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	float pos_x = (personaje->getX()) * (this->un_to_px_x) + ox;
	float pos_y = personaje->getY() * -(this->un_to_px_y) + oy;

	float x_relativa = coord_relativa(coordRel.x + (coordRel.w / 2), pos_x);
	float y_relativa = coord_relativa(coordRel.y + (coordRel.h / 2), pos_y);

	int limIzqCamera = 0;
	int limDerCamera = ancho_imagen - camera.w;
	int limInfCamera = 0;
	int limSupCamera = alto_imagen - camera.h;

	int limiteIzquierdo = 0;
	int limiteDerecho = ancho_imagen - coordRel.w
			+ (currentZoomFactor - 1) * (ancho_imagen);
	int limiteInferior = 0;
	int limiteSuperior = alto_imagen - coordRel.h
			+ (currentZoomFactor - 1) * (alto_imagen);

	if (x_relativa <= COTA_INF_X) {
		if (camera.x > limIzqCamera)
			camera.x -= FACTOR_DESPLAZAMIENTO;
		coordRel.x = camera.x * currentZoomFactor;
	} else {
		if (x_relativa >= COTA_SUP_X) {
			if (camera.x < limDerCamera)
				camera.x += FACTOR_DESPLAZAMIENTO;
			coordRel.x = camera.x * currentZoomFactor;
		}
	}
	if (y_relativa <= COTA_INF_Y) {
		if (camera.y > limInfCamera)
			camera.y -= FACTOR_DESPLAZAMIENTO;
		coordRel.y = camera.y * currentZoomFactor;
	} else if (y_relativa >= COTA_SUP_Y) {
		if (camera.y < limSupCamera)
			camera.y += FACTOR_DESPLAZAMIENTO;
		coordRel.y = camera.y * currentZoomFactor;
	}

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera,
			limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho,
			limiteInferior, limiteSuperior);
}

void Drawer::runWindow(int ancho_px, int alto_px, string imagePath) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		logSDLError(
				"Error al inicializar SDL2. Verifique la instalacion de la libreria");
		throw SDLError();
	}
	//Starting SDL2_IMAGE
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		manageSDL2_imageError();
	}

	//Opening a window
	window = SDL_CreateWindow("Snow Bros", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, ancho_px, alto_px, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		manageCreateWindowError();
	}

	//Creating a renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		manageCreateRendererError();
	}
	//Loading the image
	image = this->loadTexture(this->imagePath, renderer);
	if (image == nullptr) {
		manageLoadBackgroundError();
	}

	imagenPersonaje = this->loadTexture(SPRITE_PATH, this->renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		manageSDL_ttfError();
	}
	int sizeOfTheFont = 20;
	fontToBeUsed = TTF_OpenFont(this->fontPath.c_str(), sizeOfTheFont);
	if (fontToBeUsed == nullptr) {
		manageSDL_ttfLoadFontError();
	}

	//Aca se cargan las imagenes de las figuras.
	loadMedia();
}

void Drawer::manageSDL2_imageError() {
	logSDLError("Error al inicializar SDL2_image");
	SDL_Quit();
	throw SDLError();
}

void Drawer::manageCreateWindowError() {
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error al utilizar SDL_CreateWindow() window devolvio nullptr");
	throw SDLError();
}

void Drawer::manageCreateRendererError() {
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError(
			"Error al utilizar SDL_CreateRenderer() renderer devolvio nullptr");
	throw SDLError();
}

void Drawer::manageLoadBackgroundError() {
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError(
			"Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
	throw SDLError();
}

void Drawer::manageLoadCharacterError() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError(
			"Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
	throw SDLError();
}
void Drawer::manageSDL_ttfError() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error al inicializar SDL_ttf");
	throw SDLError();
}

void Drawer::manageSDL_ttfLoadFontError() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error al cargar la fuente de SDL_ttf");
	throw SDLError();
}

void Drawer::manageDrawMessagesError() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error de SDL_ttf al cargar escribir mensajes en la pantalla");
	throw SDLError();
}

SDL_Texture* Drawer::loadTexture(const std::string &file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	return texture;
}
void Drawer::logSDLError(const std::string &msg) {
	Log::instance()->append(msg + SDL_GetError(), Log::ERROR);
}

void Drawer::zoomIn() {
	currentZoomFactor += factor;
	int ancho_anterior = camera.w;
	int alto_anterior = camera.h;
	camera.h = alto_px / currentZoomFactor;
	camera.w = ancho_px / currentZoomFactor;

	int dif_ancho = abs(ancho_anterior - camera.w);
	int dif_alto = abs(alto_anterior - camera.h);

	camera.x += dif_ancho / 2;
	camera.y += dif_alto / 2;

	coordRel.x = currentZoomFactor * camera.x;
	coordRel.y = currentZoomFactor * camera.y;

	//Zoom in a la escala de las figuras
	un_to_px_x = un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y = un_to_px_y_inicial * currentZoomFactor;
}

void Drawer::zoomOut() {
	currentZoomFactor -= factor;
	int ancho_anterior = camera.w;
	int alto_anterior = camera.h;
	camera.h = alto_px / currentZoomFactor;
	camera.w = ancho_px / currentZoomFactor;

	int dif_ancho = abs(ancho_anterior - camera.w);
	int dif_alto = abs(alto_anterior - camera.h);

	camera.x -= dif_ancho / 2;
	camera.y -= dif_alto / 2;

	coordRel.x = currentZoomFactor * camera.x;
	coordRel.y = currentZoomFactor * camera.y;

	//Me fijo que no se expanda mas de lo que deberia
	int proximo_x_maximo = ancho_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.w
			+ ancho_anterior;
	int proximo_y_maximo = alto_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.h
			+ alto_anterior;

	if (proximo_x_maximo <= 0 || proximo_y_maximo <= 0) {
		currentZoomFactor += factor;
		return;
	}

	if (camera.x >= proximo_x_maximo) {
		camera.x = proximo_x_maximo;

	}
	if (camera.y >= proximo_y_maximo) {
		camera.y = proximo_y_maximo;

	}

	//Zoom out a la escala de las figuras
	un_to_px_x = un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y = un_to_px_y_inicial * currentZoomFactor;
}
