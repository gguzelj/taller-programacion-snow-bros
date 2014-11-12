#include "../../headers/Vista/Drawer.h"
#include "../../headers/Vista/standardEnemySprite.h"

void Drawer::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void Drawer::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
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

void Drawer::loadFont() {
	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		manageSDL_ttfError();
	}
	int sizeOfTheFont = 15;
	fontToBeUsed = TTF_OpenFont(this->fontPath.c_str(), sizeOfTheFont);
	if (fontToBeUsed == nullptr)
		manageSDL_ttfLoadFontError();
}

bool Drawer::loadMedia() {
	//Loading success flag
	bool success = true;

	//Load images
	if (!rectangleTexture.loadFromFile(rectangleImage, renderer)) {
		printf("Failed to load rectangle texture!\n");
		success = false;
	}
	if (!circleTexture.loadFromFile(circleImage, renderer)) {
		printf("Failed to load circle texture!\n");
		success = false;
	}
	if (!triangleTexture.loadFromFile(triangleImagePath, renderer)) {
		printf("Failed to load triangle texture!\n");
		success = false;
	}
	if (!squareTexture.loadFromFile(squareImagePath, renderer)) {
		printf("Failed to load square texture!\n");
		success = false;
	}
	if (!pentagonTexture.loadFromFile(pentagonImagePath, renderer)) {
		printf("Failed to load pentagon texture!\n");
		success = false;
	}
	if (!hexagonTexture.loadFromFile(hexagonImagePath, renderer)) {
		printf("Failed to load hexagon texture!\n");
		success = false;
	}
	if (!trapexTexture.loadFromFile(trapexImagePath, renderer)) {
		printf("Failed to load trapex texture!\n");
		success = false;
	}
	if (!paralelogramTexture.loadFromFile(paralelogramImagePath, renderer)) {
		printf("Failed to load paralelogram texture!\n");
		success = false;
	}
	if (!snowballTexture.loadFromFile(snowballImagePath, renderer)) {
		printf("Failed to load paralelogram texture!\n");
		success = false;
	}

	return success;
}

Drawer::Drawer() {

	this->personajeOn = {true,true,true,true,true};
	this->contadorOn = {5,5,5,5,5};

	this->renderer = nullptr;
	this->window = nullptr;
	this->image = nullptr;
	this->imagenPersonaje = nullptr;
	this->imagenPersonaje2 = nullptr;
	this->imagenPersonaje3 = nullptr;
	this->imagenPersonaje4 = nullptr;
	this->imagenPersonaje5 = nullptr;
	this->messageAboutLifes = nullptr;
	this->messageAboutPoints = nullptr;
	this->fontToBeUsed = nullptr;
	//Tamaños para dibujar el texto en pantalla. Los inicializo en 0, despues se modifican.
	this->altoText = 0;
	this->anchoPoints = 0;
	this->anchoLives = 0;

	//Paths
	this->imagePath = "resources/snowBackground.png";
	this->fontPath = "resources/dailypla.ttf";
	this->rectangleImage = "resources/textures/rectangle.png";
	this->circleImage = "resources/textures/circle.png";
	this->triangleImagePath = "resources/textures/triangle.png";
	this->squareImagePath = "resources/textures/cuadrado.png";
	this->pentagonImagePath = "resources/textures/pentagon.png";
	this->hexagonImagePath = "resources/textures/hexagon.png";
	this->trapexImagePath = "resources/textures/trapecio.png";
	this->paralelogramImagePath = "resources/textures/paralelogramo.png";
	this->snowballImagePath = "resources/textures/snowball.png";

	//Text
	this->points = "Points: ";
	this->lives = "Lives: ";

	//Hardcodeo esto por ahora.
	this->ancho_px = 1024;
	this->alto_px = 720;

	std::ifstream in(imagePath);
	unsigned int width, height;

	in.seekg(16);
	in.read((char *) &width, 4);
	in.read((char *) &height, 4);

	width = ntohl(width);
	height = ntohl(height);

	this->alto_un = height / FACTOR_CONVERSION_UN_A_PX;//Alto de la imagen dividido factor de conversion
	this->ancho_un = width / FACTOR_CONVERSION_UN_A_PX;//Ancho de la imagen dividido factor de conversion

	this->currentZoomFactor = 1.0;
	this->camera = {0,0,ancho_px,alto_px};
	this->coordRel = {0,0,ancho_px,alto_px};

	this->un_to_px_x = this->un_to_px_x_inicial = currentZoomFactor * FACTOR_CONVERSION_UN_A_PX;
	this->un_to_px_y = this->un_to_px_y_inicial = currentZoomFactor * FACTOR_CONVERSION_UN_A_PX;

	this->runWindow(ancho_px, alto_px, imagePath);
}

Drawer::~Drawer() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyTexture(imagenPersonaje2);
	SDL_DestroyTexture(imagenPersonaje3);
	SDL_DestroyTexture(imagenPersonaje4);
	SDL_DestroyTexture(imagenPersonaje5);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(messageAboutLifes);
	SDL_DestroyTexture(messageAboutPoints);
	TTF_CloseFont(fontToBeUsed);
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

void Drawer::updateView(dataFromClient_t data, char* name) {

	personaje_t personajePrincipal;
	for (unsigned int i = 0; i < data.cantPersonajes; i++) {
		if (strcmp((data.personajes[i]).id, name) == 0)
			personajePrincipal = data.personajes[i];
	}
	this->actualizarCamara(personajePrincipal);

	this->clearScenary();
	this->drawBackground();
	this->drawScenary(data, name);
	this->drawMessages(data, personajePrincipal);
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

void Drawer::drawScenary(dataFromClient_t data, char* name) {

	//Dibujamos los objetos estaticos
	for (unsigned int i = 0; i < data.cantObjEstaticos; i++)
		drawFigura(data.estaticos[i]);

	//Dibujamos los objetos dinamicos
	for (unsigned int i = 0; i < data.cantObjDinamicos; i++)
		drawFigura(data.dinamicos[i]);

	//Dibujamos los proyectiles
	for (unsigned int i = 0; i < data.gameData->cantProyectiles; i++)
		drawProyectil(data.proyectiles[i]);

	//Para probar el movimiento y demas de los enemigos los uso con el sprite de la pelota
	for (unsigned int i = 0; i < data.gameData->cantEnemigos; i++) {
		drawEnemy(data.enemigos[i]);
	}

	//Dibuja cada personaje con su sprite correspondiente
	for (unsigned int i = 0; i < data.cantPersonajes; i++) {
		//Dibujo normal
		if (data.personajes[i].connectionState >= 0) {
			if (strcmp(data.personajes[i].id, name) == 0)
				continue;
			drawCharacter(data.personajes[i], i, data.personajes[i].connectionState);
		}
	}
	//Dibujo ultimo el personaje del cliente para que se vea arriba de los demas.
	for (unsigned int i = 0; i < data.cantPersonajes; i++) {
		if (strcmp(data.personajes[i].id, name) == 0)
			drawCharacter(data.personajes[i], i, data.personajes[i].connectionState);
	}
}

/*
 * Dibuja una figura
 */
void Drawer::drawFigura(figura_t objeto) {

	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = objeto.ancho;
	float alto = objeto.alto;

	if (objeto.id == RECTANGULO_CODE) {
		rectangleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}

	if (objeto.id == CIRCULO_CODE) {
		float radio = objeto.alto / 2;

		circleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - radio) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + radio) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}

	if (objeto.id == TRIANGULO_CODE) {
		SDL_Point centro;
		centro.x = ancho / 2 * un_to_px_x;
		centro.y = alto * un_to_px_y / 1.5;

		triangleTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 1.5) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, &centro);
	}

	if (objeto.id == CUADRADO_CODE) {
		squareTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}

	if (objeto.id == PENTAGONO_CODE) {
		SDL_Point centro;
		centro.x = (ancho * un_to_px_x) / 2;
		centro.y = (ancho * un_to_px_y) / 1.91;

		pentagonTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox),
				coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / (1 + cos(M_PI / 5))) + oy), ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, &centro);
	}

	if (objeto.id == HEXAGONO_CODE) {
		hexagonTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}

	if (objeto.id == TRAPECIO_CODE) {
		trapexTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 1.6) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy),
				ancho * un_to_px_x * 1.1, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == PARALELOGRAMO_CODE) {
		paralelogramTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox), coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
}

/*
 * Dibujo los proyectiles
 */
void Drawer::drawProyectil(proyectil_t proy) {

	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = proy.ancho;
	float alto = proy.alto;

	if (proy.id == BOLA_NIEVE_CODE) {
		float radio = proy.alto / 2;

		snowballTexture.render(renderer, coord_relativa(coordRel.x, un_to_px_x * (proy.centro.x - radio) + ox), coord_relativa(coordRel.y, -un_to_px_y * (proy.centro.y + radio) + oy),
				ancho * un_to_px_x, alto * un_to_px_y, nullptr, proy.rotacion * -RADTODEG, nullptr);
	}
}

void Drawer::drawEnemy(enemigo_t enemigo) {

	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = enemigo.ancho;
	float alto = enemigo.alto;

	char codigo_estado = enemigo.estado;
	char orientacion = enemigo.orientacion;


	int pos_x = coord_relativa(coordRel.x, un_to_px_x * (enemigo.centro.x ) + ox);
	int pos_y = coord_relativa(coordRel.y, -un_to_px_y * (enemigo.centro.y) + oy);

	if(enemigo.nivelDeCongelamiento == 7)
		drawCongelamiento(this->renderer, imagenEnemigos, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y,enemigo.nivelDeCongelamiento);
	else{
		switch (codigo_estado) {
		case JUMPING:
			drawStandardEnemyJumping(this->renderer, imagenEnemigos, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case STANDBY:
			drawStandardEnemyStandBy(renderer, imagenEnemigos, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case WALKING:
			drawStandardEnemyWalking(renderer, imagenEnemigos, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case FALLING:
			drawStandardEnemyFalling(renderer, imagenEnemigos, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		}
		drawCongelamiento(this->renderer, imagenEnemigos, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y,enemigo.nivelDeCongelamiento);
	}
}

bool Drawer::blinkCharacter(personaje_t person, int index) {

	if (!person.inmune)
		return false;

	contadorOn[index]--;
	if (contadorOn[index] == 0) {
		personajeOn[index] = !personajeOn[index];
		contadorOn[index] = 5;
	}

	return personajeOn[index];
}

void Drawer::drawCharacter(personaje_t person, int index, int connectionState) {

	if (blinkCharacter(person, index))
		return;

	//insert framebreak for constant framerate here

	int ancho_imagen = (this->ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (this->alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	char codigo_estado = person.estado;
	char orientacion = person.orientacion;

	float pos_x = (person.centro.x) * (this->un_to_px_x) + ox;
	float pos_y = (person.centro.y) * -(this->un_to_px_y) + oy;
	pos_x = coord_relativa(coordRel.x, pos_x);
	pos_y = coord_relativa(coordRel.y, pos_y);

	SDL_Texture *textura;

	if (connectionState == 1)
		textura = selectTexture(index);	//Elijo una textura para el jugador normal
	else if (connectionState == 0)
		textura = imagenPersonaje5;	//Selecciono la textura waiting

	switch (codigo_estado) {
	case JUMPING:
		drawPersonajeJumping(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case STANDBY:
		drawPersonajeStandBy(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case WALKING:
		drawPersonajeWalking(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case FALLING:
		drawPersonajeFalling(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case SHOOTING:
		drawPersonajeShooting(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case DYING:
		drawPersonajeMuriendo(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case PUSHING:
		drawPersonajeEmpujando(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	case KICKING:
		drawPersonajePateando(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
		break;
	}
}

void Drawer::drawMessages(dataFromClient_t data, personaje_t personaje) {

//Set the coordinates which we want to draw to
	float coordXDelMensaje = 10; //Por ahora lo puse asi, despues lo acomodamos bien con los demas mensajes.
	float coordYDelMensaje = 10; //Parte superior de la pantalla

	SDL_Color textColor = { 0, 0, 0, 0xFF };

	pointsT.loadFromRenderedText(renderer, fontToBeUsed, points + std::to_string(personaje.points), textColor, &anchoPoints, &altoText);
	livesT.loadFromRenderedText(renderer, fontToBeUsed, lives + std::to_string(personaje.lives), textColor, &anchoLives, &altoText);

//Render the first message
	pointsT.render(renderer, coordXDelMensaje, coordYDelMensaje, anchoPoints, altoText);

//Render the other message
	coordYDelMensaje += altoText + 5;
	livesT.render(renderer, coordXDelMensaje, coordYDelMensaje, anchoLives, altoText);

//Dibujamos la pantalla de espera
	if (data.gameData->paused)
		drawWaitingScreen();

}

void Drawer::drawWaitingScreen() {

	std::string texto = "Esperando a todos los jugadores";

	int anchoT = 500;
	int altoT = 100;

//Pense que esto lo dibujaba en medio de la pantall, pero no..
	float ox = 300;
	float oy = 300;

	SDL_Color textColor = { 0, 0, 0, 0xFF };

	waitingScreenT.loadFromRenderedText(renderer, fontToBeUsed, texto, textColor, &anchoT, &altoT);
	waitingScreenT.render(renderer, ox, oy, anchoT, altoT);

}

void Drawer::presentScenary() {
	SDL_RenderPresent(this->renderer);
}

void ajusteFueraDeLimite(SDL_Rect &rect, int limIzq, int limDer, int limInf, int limSup) {
	if (rect.y < limInf)
		rect.y = limInf;
	if (rect.y > limSup)
		rect.y = limSup;
	if (rect.x < limIzq)
		rect.x = limIzq;
	if (rect.x > limDer)
		rect.x = limDer;
}

void Drawer::inicializarCamara(personaje_t personaje) {
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	float pos_x = (personaje.centro.x) * (this->un_to_px_x) + ox;
	float pos_y = personaje.centro.y * -(this->un_to_px_y) + oy;

	camera.x = pos_x - camera.w / 2;
	camera.y = pos_y - camera.h / 2;

	coordRel.x = camera.x;
	coordRel.y = camera.y;

	int limIzqCamera = 0;
	int limDerCamera = ancho_imagen - camera.w;
	int limInfCamera = 0;
	int limSupCamera = alto_imagen - camera.h;

	int limiteIzquierdo = 0;
	int limiteDerecho = ancho_imagen - coordRel.w + (currentZoomFactor - 1) * (ancho_imagen);
	int limiteInferior = 0;
	int limiteSuperior = alto_imagen - coordRel.h + (currentZoomFactor - 1) * (alto_imagen);

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera, limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho, limiteInferior, limiteSuperior);

}

void Drawer::actualizarCamara(personaje_t personaje) {
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	float pos_x = (personaje.centro.x) * (this->un_to_px_x) + ox;
	float pos_y = personaje.centro.y * -(this->un_to_px_y) + oy;

	float x_relativa = coord_relativa(coordRel.x + (coordRel.w / 2), pos_x);
	float y_relativa = coord_relativa(coordRel.y + (coordRel.h / 2), pos_y);

	int limIzqCamera = 0;
	int limDerCamera = ancho_imagen - camera.w;
	int limInfCamera = 0;
	int limSupCamera = alto_imagen - camera.h;

	int limiteIzquierdo = 0;
	int limiteDerecho = ancho_imagen - coordRel.w + (currentZoomFactor - 1) * (ancho_imagen);
	int limiteInferior = 0;
	int limiteSuperior = alto_imagen - coordRel.h + (currentZoomFactor - 1) * (alto_imagen);

	if (x_relativa <= COTA_INF_X) {
		if (camera.x > limIzqCamera)
			camera.x -= abs(x_relativa - COTA_INF_X);
		coordRel.x = camera.x * currentZoomFactor;
	} else {
		if (x_relativa >= COTA_SUP_X) {
			if (camera.x < limDerCamera)
				camera.x += abs(x_relativa - COTA_SUP_X);
			coordRel.x = camera.x * currentZoomFactor;
		}
	}
	if (y_relativa <= COTA_INF_Y) {
		if (camera.y > limInfCamera)
			camera.y -= abs(y_relativa - COTA_INF_Y);
		coordRel.y = camera.y * currentZoomFactor;
	} else if (y_relativa >= COTA_SUP_Y) {
		if (camera.y < limSupCamera)
			camera.y += abs(y_relativa - COTA_SUP_Y);
		coordRel.y = camera.y * currentZoomFactor;
	}

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera, limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho, limiteInferior, limiteSuperior);
}

void Drawer::runWindow(int ancho_px, int alto_px, string imagePath) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		logSDLError("Error al inicializar SDL2. Verifique la instalacion de la libreria");
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

	imagenPersonaje2 = this->loadTexture(SPRITE2_PATH, this->renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}

	imagenPersonaje3 = this->loadTexture(SPRITE3_PATH, this->renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}

	imagenPersonaje4 = this->loadTexture(SPRITE4_PATH, this->renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}

	imagenPersonaje5 = this->loadTexture(SPRITE5_PATH, this->renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}

	imagenEnemigos = this->loadTexture(ENEMY_SPRITE_PATH, this->renderer);
		if (imagenEnemigos == nullptr) {
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

//Aca se carga la fuente
	loadFont();

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
	logSDLError("Error al utilizar SDL_CreateRenderer() renderer devolvio nullptr");
	throw SDLError();
}

void Drawer::manageLoadBackgroundError() {
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
	throw SDLError();
}

void Drawer::manageLoadCharacterError() {
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	logSDLError("Error al utilizar IMG_LoadTexture. Verifique el path de la imagen.");
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
	Log::ins()->add(msg + SDL_GetError(), Log::ERROR);
}

void Drawer::zoomIn() {
	if (currentZoomFactor < ZOOM_MAX) {
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
}

void Drawer::zoomOut() {
	currentZoomFactor -= factor;
	int ancho_anterior = camera.w;
	int alto_anterior = camera.h;
	camera.h = alto_px / currentZoomFactor;
	camera.w = ancho_px / currentZoomFactor;

	int dif_ancho = abs(ancho_anterior - camera.w);
	int dif_alto = abs(alto_anterior - camera.h);

//Me fijo que no se expanda mas de lo que deberia
	int proximo_x_maximo = ancho_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.w + ancho_anterior;
	int proximo_y_maximo = alto_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.h + alto_anterior;

	if (proximo_x_maximo <= 0 || proximo_y_maximo <= 0) {
		currentZoomFactor += factor;
		camera.h = alto_px / currentZoomFactor;
		camera.w = ancho_px / currentZoomFactor;
		return;
	}

	camera.x -= dif_ancho / 2;
	camera.y -= dif_alto / 2;

	if (camera.x >= proximo_x_maximo) {
		camera.x = proximo_x_maximo;
	}
	if (camera.y >= proximo_y_maximo) {
		camera.y = proximo_y_maximo;
	}

	coordRel.x = currentZoomFactor * camera.x;
	coordRel.y = currentZoomFactor * camera.y;

//Zoom out a la escala de las figuras
	un_to_px_x = un_to_px_x_inicial * currentZoomFactor;
	un_to_px_y = un_to_px_y_inicial * currentZoomFactor;
}

SDL_Texture* Drawer::selectTexture(int index) throw (ErrorFueraDeRango) {
	if (index >= 4 || index < 0)
		throw ErrorFueraDeRango();

	if (index == 0)
		return imagenPersonaje;

	else if (index == 1)
		return imagenPersonaje2;

	else if (index == 2)
		return imagenPersonaje3;

	else {
		return imagenPersonaje4;
	}
}
