#include "../../headers/Vista/Drawer.h"
#include "../../headers/Vista/standardEnemySprite.h"

Drawer::Drawer() {
	this->personajeOn = {true,true,true,true,true};
	this->contadorOn = {5,5,5,5,5};

	this->pasandoDeNivel = false;

	this->renderer = nullptr;
	this->window = nullptr;
	this->image = nullptr;
	this->waitingImage = nullptr;
	this->winningImage = nullptr;
	this->imagenPersonaje = nullptr;
	this->imagenPersonaje2 = nullptr;
	this->imagenPersonaje3 = nullptr;
	this->imagenPersonaje4 = nullptr;
	this->imagenPersonaje5 = nullptr;
	this->imagenEnemigos = nullptr;
	this->imagenEnemigoFuego = nullptr;
	this->congelamientoUno = nullptr;
	this->congelamientoDos = nullptr;
	this->congelamientoTres = nullptr;
	this->congelamientoCuatro = nullptr;
	this->fontToBeUsed = nullptr;
	this->portal = nullptr;

	//The music that will be played
	gMusic = nullptr;
	gShooting = nullptr;
	gJumping = nullptr;
	gDying = nullptr;
	gBallBreaking = nullptr;
	gBonus = nullptr;
	gOneUp = nullptr;
	gGameover = nullptr;
	gTransition = nullptr;
	gkicking = nullptr;

	//Tamaños para dibujar el texto en pantalla. Los inicializo en 0, despues se modifican.
	this->altoText = 0;
	this->anchoPoints = 0;
	this->anchoLives = 0;
	this->anchoLevel = 0;
	this->anchoNumber = 0;
	this->altoWaiting = 0;
	this->anchoWaiting = 0;

	//Limites
	this->limDerCamera = 0;
	this->limInfCamera = 0;
	this->limIzqCamera = 0;
	this->limSupCamera = 0;
	this->limiteDerecho = 0;
	this->limiteInferior = 0;
	this->limiteIzquierdo = 0;
	this->limiteSuperior = 0;

	//Paths
	this->imagePath = "resources/background1.png";
	this->fontPath = "resources/dailypla.ttf";
	this->rectangleImage = "resources/textures/rectangle.png";
	this->icePlatformImage = "resources/textures/icePlatform.png";
	this->circleImage = "resources/textures/circle.png";
	this->triangleImagePath = "resources/textures/triangle.png";
	this->squareImagePath = "resources/textures/cuadrado.png";
	this->pentagonImagePath = "resources/textures/pentagon.png";
	this->hexagonImagePath = "resources/textures/hexagon.png";
	this->trapexImagePath = "resources/textures/trapecio.png";
	this->paralelogramImagePath = "resources/textures/paralelogramo.png";
	this->snowballImagePath = "resources/textures/snowball.png";
	this->portalballImagePath = "resources/textures/portalball.png";
	this->fireballImagePath = "resources/textures/bolaDeFuego.png";
	this->bonusPortalPath = "resources/textures/BonusPortal.png";
	this->bonusPotenciaPath = "resources/textures/BonusPotencia.png";
	this->bonusVelocidadPath = "resources/textures/BonusVelocidad.png";
	this->bonusVidaPath = "resources/textures/BonusVida.png";
	this->portalPath = "resources/sprites/portal.png";
	this->gameOverScreenPath = "resources/textures/gameOver.png";
	this->waitingScreenPath = "resources/textures/waitingBackground.png";
	this->winningScreenPath = "resources/textures/winningBackground.png";

	//Text
	this->points = "Points: ";
	this->lives = "Lives: ";
	this->level = "Level ";

	//Hardcodeo esto por ahora.
	this->ancho_px = 1024;
	this->alto_px = 690;

	this->puntaje = 0;

	std::ifstream in(imagePath);
	unsigned int width, height;

	in.seekg(16);
	in.read((char *) &width, 4);
	in.read((char *) &height, 4);

	width = ntohl(width);
	height = ntohl(height);

	this->alto_un = (float)height / FACTOR_CONVERSION_UN_A_PX;//Alto de la imagen dividido factor de conversion
	this->ancho_un = (float) width / FACTOR_CONVERSION_UN_A_PX;//Ancho de la imagen dividido factor de conversion
	this->currentZoomFactor = 1.0;
	this->camera = {0,0,ancho_px / currentZoomFactor,alto_px / (currentZoomFactor)};
	this->coordRel = {0,0,ancho_px,alto_px};

	this->nivel = 1;
	this->setearLimitesDelNivel(nivel);

	this->un_to_px_x = this->un_to_px_x_inicial = currentZoomFactor * FACTOR_CONVERSION_UN_A_PX;
	this->un_to_px_y = this->un_to_px_y_inicial = currentZoomFactor * FACTOR_CONVERSION_UN_A_PX;

	this->runWindow(ancho_px, alto_px, imagePath);
}

Drawer::~Drawer() {
	Mix_FreeMusic(gMusic);
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(waitingImage);
	SDL_DestroyTexture(winningImage);
	SDL_DestroyTexture(imagenPersonaje);
	SDL_DestroyTexture(imagenPersonaje2);
	SDL_DestroyTexture(imagenPersonaje3);
	SDL_DestroyTexture(imagenPersonaje4);
	SDL_DestroyTexture(imagenPersonaje5);
	SDL_DestroyTexture(imagenEnemigos);
	SDL_DestroyTexture(imagenEnemigoFuego);
	SDL_DestroyTexture(congelamientoUno);
	SDL_DestroyTexture(congelamientoDos);
	SDL_DestroyTexture(congelamientoTres);
	SDL_DestroyTexture(congelamientoCuatro);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(fontToBeUsed);
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}

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

void Drawer::updateView(dataFromClient_t data, char* name) {

	personaje_t personajePrincipal;
	for (unsigned int i = 0; i < data.cantPersonajes; i++) {
		if (strcmp((data.personajes[i]).id, name) == 0)
			personajePrincipal = data.personajes[i];
	}
	this->actualizarCamara(personajePrincipal);

	this->clearScenary();
	//Evaluamos si dibujar la pantalla de espera o el escenario normal
	if (data.gameData->paused && !data.gameData->gameOver && !data.gameData->won)
		drawWaitingScreen();

	else if(data.gameData->won)
		drawWinningScreen(data);

	else{
		this->drawBackground();
		this->drawScenary(data, name);
		this->drawMessages(data, personajePrincipal);
	}
	this->presentScenary();
}

void Drawer::reproducirSonidos(int* &sonidos, unsigned int size) {
	for(unsigned int i = 0; i < size; i++){
		if(sonidos[i] == SHOOTING){
			Mix_PlayChannel( -1, gShooting, 0 );
		}
		else if(sonidos[i] == JUMPING){
			Mix_PlayChannel( -1, gJumping, 0 );
		}
		else if(sonidos[i] == DYING){
			Mix_PlayChannel( -1, gDying, 0 );
		}
		else if(sonidos[i] == BALLBREAKING){
			Mix_PlayChannel( -1, gBallBreaking, 0 );
		}
		else if(sonidos[i] == BONUS){
			Mix_PlayChannel( -1, gBonus, 0 );
		}
		else if(sonidos[i] == ONEUP){
			Mix_PlayChannel( -1, gOneUp, 0 );
		}
		else if(sonidos[i] == GAMEOVER){
			Mix_PlayChannel( -1, gGameover, 0 );
		}
		else if(sonidos[i] == KICKING){
			Mix_PlayChannel( -1, gkicking, 0 );
		}
	}
}

// ############################### //
// ##### Auxiliray functions ##### //
// ############################### //

float coord_relativa(float referencia, float coord) {
	return coord - referencia;
}

int anchoPersonaje(float un_to_px_x) {
	return (MITAD_ANCHO_PERSONAJE * 2) * un_to_px_x;
}
int altoPersonaje(float un_to_px_y) {
	return ((MITAD_ALTO_PERSONAJE * 2) * un_to_px_y);
}

// ########################### //
// ##### Private methods ##### //
// ########################### //
void Drawer::clearScenary() {
	SDL_RenderClear(renderer);
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
		if (strcmp(data.personajes[i].id, name) == 0){
			drawCharacter(data.personajes[i], i, data.personajes[i].connectionState);
			if(data.personajes[i].points != 0)
				puntaje = data.personajes[i].points;
		}
	}
}

/*
 * Dibuja una figura
 */
void Drawer::drawFigura(figura_t objeto) {

	int ancho_imagen = (ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (alto_un * FACTOR_CONVERSION_UN_A_PX);

	float ancho = objeto.ancho;
	float alto = objeto.alto;

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	int pos_x = coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 2) + ox);
	int pos_y = coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 2) + oy);


	if (objeto.id == RECTANGULO_CODE) {
		rectangleLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if(objeto.id == PLATAFORMAHIELO_CODE) {
		icePlatformLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == CIRCULO_CODE) {
		circleLT.render(renderer, pos_x, pos_y,	ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == TRIANGULO_CODE) {
		SDL_Point centro;
		centro.x = ancho / 2 * un_to_px_x;
		centro.y = alto * un_to_px_y / 1.5;
		pos_y = coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / 1.5) + oy);

		triangleLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, &centro);
	}
	if (objeto.id == CUADRADO_CODE) {
		squareLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == PENTAGONO_CODE) {
		SDL_Point centro;
		centro.x = (ancho * un_to_px_x) / 2;
		centro.y = (ancho * un_to_px_y) / 1.91;
		pos_y = coord_relativa(coordRel.y, -un_to_px_y * (objeto.centro.y + alto / (1 + cos(M_PI / 5))) + oy);

		pentagonLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, &centro);
	}
	if (objeto.id == HEXAGONO_CODE) {
		hexagonLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == TRAPECIO_CODE) {
		pos_x = coord_relativa(coordRel.x, un_to_px_x * (objeto.centro.x - ancho / 1.6) + ox);
		trapexLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x * 1.1, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if (objeto.id == PARALELOGRAMO_CODE) {
		paralelogramLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if(objeto.id == PORTAL_CODE){
		drawPortal(renderer, portal, pos_x - ANCHO_PORTAL / 2 - ancho * un_to_px_x / 2, pos_y, ANCHO_PORTAL, alto * un_to_px_y);
	}
	if(objeto.id == BONUS_PORTAL_CODE){
		bonusPortalLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if(objeto.id == BONUS_POTENCIA_CODE){
		bonusPotenciaLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if(objeto.id == BONUS_VELOCIDAD_CODE){
		bonusVelocidadLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
	if(objeto.id == BONUS_VIDA_CODE){
		bonusVidaLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, objeto.rotacion * -RADTODEG, nullptr);
	}
}

/*
 * Dibujo los proyectiles
 */
void Drawer::drawProyectil(proyectil_t proy) {

	int ancho_imagen = (ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = proy.ancho;
	float alto = proy.alto;

	int pos_x = coord_relativa(coordRel.x, un_to_px_x * (proy.centro.x - ancho / 2) + ox);
	int pos_y = coord_relativa(coordRel.y, -un_to_px_y * (proy.centro.y + alto / 2) + oy);

	if (proy.id == BOLA_NIEVE_CODE) {
		snowballLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, proy.rotacion, nullptr);
	}
	else if (proy.id == BOLA_PORTAL_CODE) {
		portalballLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, proy.rotacion, nullptr);
	}
	else if (proy.id == BOLA_FUEGO_CODE) {
		fireballLT.render(renderer, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y, nullptr, proy.rotacion, nullptr);
	}
}

void Drawer::drawEnemy(enemigo_t enemigo) {
	if(enemigo.tipo == ID_ENEMIGO_BASICO){
		drawStandardEnemy(enemigo);
	}
	else if(enemigo.tipo == ID_ENEMIGO_FUEGO){
		drawFireEnemy(enemigo);
	}
}

void Drawer::drawStandardEnemy(enemigo_t enemigo){
	SDL_Texture* imagen;
	imagen = imagenEnemigos;

	int ancho_imagen = (ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = enemigo.ancho;
	float alto = enemigo.alto;

	char codigo_estado = enemigo.estado;
	char orientacion = enemigo.orientacion;

	int pos_x = coord_relativa(coordRel.x, un_to_px_x * (enemigo.centro.x ) + ox);
	int pos_y = coord_relativa(coordRel.y, -un_to_px_y * (enemigo.centro.y) + oy);

	if(enemigo.nivelDeCongelamiento == 7)
		drawCongelamiento(renderer, congelamientoCuatro, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
	else{
		switch (codigo_estado) {
		case JUMPING:
			drawStandardEnemyJumping(renderer, imagen, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case STANDBY:
			drawStandardEnemyStandBy(renderer, imagen, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case WALKING:
			drawStandardEnemyWalking(renderer, imagen, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case FALLING:
			drawStandardEnemyFalling(renderer, imagen, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		}
		if(enemigo.nivelDeCongelamiento >0){
			if(enemigo.nivelDeCongelamiento < 3){
				drawCongelamiento(renderer, congelamientoUno, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			}
			else{
				if( enemigo.nivelDeCongelamiento <5){
					drawCongelamiento(renderer, congelamientoDos, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
				}
				else{
					if( enemigo.nivelDeCongelamiento < 7){
						drawCongelamiento(renderer, congelamientoTres, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
					}
				}
			}
		}
	}
}

void Drawer::drawFireEnemy(enemigo_t enemigo){
	SDL_Texture* imagen;
	imagen = imagenEnemigos;

	int ancho_imagen = (ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen * currentZoomFactor) / 2;
	int oy = (alto_imagen * currentZoomFactor) / 2;

	float ancho = enemigo.ancho;
	float alto = enemigo.alto;

	char codigo_estado = enemigo.estado;
	char orientacion = enemigo.orientacion;

	int pos_x = coord_relativa(coordRel.x, un_to_px_x * (enemigo.centro.x ) + ox);
	int pos_y = coord_relativa(coordRel.y, -un_to_px_y * (enemigo.centro.y) + oy);

	if(enemigo.nivelDeCongelamiento == 7)
		drawCongelamiento(renderer, congelamientoCuatro, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);

	else{
		switch (codigo_estado) {
		case STANDBY:
			drawFireEnemyStandBy(renderer, imagen, orientacion, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case WALKING:
			drawFireEnemyWalking(renderer, imagen, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
			drawFireEnemyFalling(renderer, imagen, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		case SHOOTING:
			drawFireEnemyShooting(renderer, imagen, orientacion, pos_x, pos_y,  ancho * un_to_px_x, alto * un_to_px_y);
			break;
		}
		if(enemigo.nivelDeCongelamiento >0){
			if(enemigo.nivelDeCongelamiento < 3){
				drawCongelamiento(renderer, congelamientoUno, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
			}
			else{
				if( enemigo.nivelDeCongelamiento <5){
					drawCongelamiento(renderer, congelamientoDos, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
				}
				else{
					if( enemigo.nivelDeCongelamiento < 7){
						drawCongelamiento(renderer, congelamientoTres, pos_x, pos_y, ancho * un_to_px_x, alto * un_to_px_y);
					}
				}
			}
		}
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

	int ancho_imagen = (ancho_un * FACTOR_CONVERSION_UN_A_PX);
	int alto_imagen = (alto_un * FACTOR_CONVERSION_UN_A_PX);

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	char codigo_estado = person.estado;
	char orientacion = person.orientacion;

	float pos_x = (person.centro.x) * (un_to_px_x) + ox;
	float pos_y = (person.centro.y) * -(un_to_px_y) + oy;
	pos_x = coord_relativa(coordRel.x, pos_x);
	pos_y = coord_relativa(coordRel.y, pos_y);

	SDL_Texture *textura;

	if (connectionState == 1)
		textura = selectTexture(person.color);	//Elijo una textura para el jugador normal
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
	case FLYING:
		drawPersonajeVolando(renderer, textura, orientacion, pos_x, pos_y, anchoPersonaje(un_to_px_x), altoPersonaje(un_to_px_y));
	}
}

void Drawer::drawMessages(dataFromClient_t data, personaje_t personaje) {

	//Set the coordinates which we want to draw to
	float coordXDelMensaje = 5; //Por ahora lo puse asi, despues lo acomodamos bien con los demas mensajes.
	float coordYDelMensaje = 5; //Parte superior de la pantalla

	if(data.cantPersonajes >0){
		int desplazamientoEnX = (ancho_px - (anchoLives + anchoNumber*5)) / data.cantPersonajes;

		for (unsigned int p = 0; p < data.cantPersonajes; p++){
			if (data.personajes[p].connectionState >= 0){
				int puntos = data.personajes[p].points;
				int vidas = data.personajes[p].lives;

				//Render the first message
				string name = string(data.personajes[p].id);
				for(int i = 0; i < name.size(); i++)
					letrasLT[name[i]-'A']->render(renderer, coordXDelMensaje+anchoNumber*i, coordYDelMensaje, anchoNumber, altoText);
				coordYDelMensaje += altoText + 5;
				pointsLT.render(renderer, coordXDelMensaje, coordYDelMensaje, anchoPoints, altoText);
				coordXDelMensaje += anchoPoints;
				for(int i = 5; i > 0; i--){
					numerosLT[puntos%10]->render(renderer, coordXDelMensaje+anchoNumber*(i-1), coordYDelMensaje, anchoNumber, altoText);
					puntos/=10;
				}

				//Render the other message
				coordYDelMensaje += altoText + 5;
				coordXDelMensaje -= anchoPoints;
				livesLT.render(renderer, coordXDelMensaje, coordYDelMensaje, anchoLives, altoText);
				coordXDelMensaje += anchoLives;
				numerosLT[vidas]->render(renderer, coordXDelMensaje, coordYDelMensaje, anchoNumber, altoText);

				//Corijo la posicion para el proximo personaje
				coordXDelMensaje += desplazamientoEnX - anchoLives;
				coordYDelMensaje -= 2* (altoText + 5);
			}
		}
	}
	//Dibujamos el indicador de nivel
	coordXDelMensaje = (ancho_px - (anchoLives + anchoNumber*5)) / 2;
	coordYDelMensaje = altoText + 60;
	levelsLT.render(renderer, coordXDelMensaje, coordYDelMensaje, anchoPoints, altoText);
	coordXDelMensaje += anchoPoints;
	numerosBlackLT[data.gameData->nivel]->render(renderer, coordXDelMensaje, coordYDelMensaje, anchoNumber, altoText);

	//Dibujamos la pantalla de gameOver
	if (data.gameData->gameOver){
		drawGameOverScreen();
	}
}

void Drawer::drawWaitingScreen() {
	renderTexture(waitingImage, renderer, 0, 0);
}

void Drawer::drawWinningScreen(dataFromClient_t data){
	renderTexture(winningImage, renderer, 0, 0);

	//Mostramos el puntaje obtenido
	//Set the coordinates which we want to draw to
	float coordXDelMensaje = 670;
	float coordYDelMensaje = 400;

	int puntos = puntaje;
	//Render the first message
	for(int i = 5; i > 0; i--){
		numerosLT[puntos%10]->render(renderer, coordXDelMensaje+anchoNumber*(i-1), coordYDelMensaje, anchoNumber, altoText);
		puntos/=10;
	}
}

void Drawer::drawGameOverScreen(){
	int anchoT = 500;
	int altoT = 100;
	float ox = (ancho_px/2)-(anchoT/2);
	float oy = 300;
	gameOverScreenLT.render(renderer, ox, oy, anchoT, altoT);
}

void Drawer::presentScenary() {
	SDL_RenderPresent(renderer);
}

void Drawer::transicionNivel(){
	this->pasandoDeNivel = true;
	Mix_PlayChannel( -1, gTransition, 0 );
	std::cerr<< (this->nivel) +1<<std::endl;
	this->setearLimiteInferiorDelNivel((this->nivel)+1);
}

void Drawer::setearLimiteInferiorDelNivel(unsigned int nivel){

	if(nivel == 1){
		limInfCamera = 750;
		limiteInferior = 750 *currentZoomFactor ;
	}
	if(nivel == 2){
		limInfCamera = 0;
		limiteInferior = 0 ;
	}
}

void Drawer::setearLimitesDelNivel(unsigned int nivel){
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;
	limIzqCamera = 0;
	limDerCamera = ancho_imagen -camera.w;
	limiteIzquierdo = 0;
	limiteDerecho = ancho_imagen - coordRel.w + (currentZoomFactor - 1) * (ancho_imagen);

	if(nivel == 1){
		limSupCamera = alto_imagen - camera.h;
		limiteSuperior =  alto_imagen - coordRel.h + (currentZoomFactor - 1) * (alto_imagen);
	}
	if(nivel == 2){
		limSupCamera = 690 - camera.h;
		limiteSuperior = 690 * currentZoomFactor - coordRel.h;
	}
	this->setearLimiteInferiorDelNivel(nivel);
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

	float pos_x = (personaje.centro.x) * (un_to_px_x) + ox;
	float pos_y = personaje.centro.y * -(un_to_px_y) + oy;

	camera.x = pos_x - camera.w / 2;
	camera.y = pos_y - camera.h / 2;

	coordRel.x = camera.x;
	coordRel.y = camera.y;

	ajusteFueraDeLimite(camera, limIzqCamera, limDerCamera, limInfCamera, limSupCamera);
	ajusteFueraDeLimite(coordRel, limiteIzquierdo, limiteDerecho, limiteInferior, limiteSuperior);
}

void Drawer::actualizarCamara(personaje_t personaje) {
	float ancho_imagen = ancho_un * FACTOR_CONVERSION_UN_A_PX;
	float alto_imagen = alto_un * FACTOR_CONVERSION_UN_A_PX;

	int ox = (ancho_imagen / 2) + (currentZoomFactor - 1) * (ancho_imagen) / 2;
	int oy = (alto_imagen / 2) + (currentZoomFactor - 1) * (alto_imagen) / 2;

	float pos_x = (personaje.centro.x) * (un_to_px_x) + ox;
	float pos_y = personaje.centro.y * -(un_to_px_y) + oy;

	float x_relativa = coord_relativa(coordRel.x + (coordRel.w / 2), pos_x);
	float y_relativa = coord_relativa(coordRel.y + (coordRel.h / 2), pos_y);

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

void Drawer::zoomIn() {
	if (currentZoomFactor < ZOOM_MAX) {
		currentZoomFactor += factor;
		int ancho_anterior = camera.w;
		int alto_anterior = camera.h;
		camera.h = alto_px / currentZoomFactor;
		camera.w = ancho_px / (currentZoomFactor);

		int dif_ancho = abs(ancho_anterior - camera.w);
		int dif_alto = abs(alto_anterior - camera.h);

		camera.x += dif_ancho / 2;
		camera.y += dif_alto / 2;

		coordRel.x = currentZoomFactor * camera.x;
		coordRel.y = currentZoomFactor * camera.y;

		//Zoom in a la escala de las figuras
		un_to_px_x = FACTOR_CONVERSION_UN_A_PX * currentZoomFactor;
		un_to_px_y =  FACTOR_CONVERSION_UN_A_PX * currentZoomFactor;
		setearLimitesDelNivel(nivel);
	}
}

void Drawer::zoomOut() {
	currentZoomFactor -= factor;
	int ancho_anterior = camera.w;
	int alto_anterior = camera.h;
	camera.h = alto_px / (currentZoomFactor);
	camera.w = ancho_px / currentZoomFactor;

	int dif_ancho = abs(ancho_anterior - camera.w);
	int dif_alto = abs(alto_anterior - camera.h);

	//Me fijo que no se expanda mas de lo que deberia
	int proximo_x_maximo = ancho_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.w + ancho_anterior;
	int proximo_y_maximo = alto_un * FACTOR_CONVERSION_UN_A_PX - 2 * camera.h + alto_anterior;

	if (proximo_x_maximo <= limIzqCamera || proximo_y_maximo <= limInfCamera) {
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
	un_to_px_x = FACTOR_CONVERSION_UN_A_PX * currentZoomFactor;
	un_to_px_y =  FACTOR_CONVERSION_UN_A_PX * currentZoomFactor;
	setearLimitesDelNivel(nivel);
}

SDL_Texture* Drawer::selectTexture(char color) throw (ErrorFueraDeRango) {
	if (color == COLOR_BLANCO)
		return imagenPersonaje;

	else if (color == COLOR_AZUL)
		return imagenPersonaje2;

	else if (color == COLOR_ROJO)
		return imagenPersonaje3;

	else if (color == COLOR_VERDE)
		return imagenPersonaje4;

	else
		throw ErrorFueraDeRango();
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

	//Loading the images
	image = loadTexture(imagePath, renderer);
	if (image == nullptr) {
		manageLoadBackgroundError();
	}
	waitingImage = loadTexture(waitingScreenPath, renderer);
	if (waitingImage == nullptr) {
		manageLoadBackgroundError();
	}
	winningImage = loadTexture(winningScreenPath, renderer);
	if (winningImage == nullptr) {
		manageLoadBackgroundError();
	}
	imagenPersonaje = loadTexture(SPRITE_PATH, renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}
	imagenPersonaje2 = loadTexture(SPRITE2_PATH, renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}
	imagenPersonaje3 = loadTexture(SPRITE3_PATH, renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}
	imagenPersonaje4 = loadTexture(SPRITE4_PATH, renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}
	imagenPersonaje5 = loadTexture(SPRITE5_PATH, renderer);
	if (imagenPersonaje == nullptr) {
		manageLoadCharacterError();
	}
	imagenEnemigos = loadTexture(ENEMY_SPRITE_PATH, renderer);
	if (imagenEnemigos == nullptr) {
		manageLoadCharacterError();
	}
	imagenEnemigoFuego = loadTexture(ENEMIGO_FUEGO_SPRITE_PATH, renderer);
	if (imagenEnemigoFuego == nullptr) {
		manageLoadCharacterError();
	}
	congelamientoUno = loadTexture(CONGELAMIENTO_NIVEL_UNO_PATH, renderer);
	if (congelamientoUno == nullptr) {
		manageLoadCharacterError();
	}
	congelamientoDos = loadTexture(CONGELAMIENTO_NIVEL_DOS_PATH, renderer);
	if (congelamientoDos == nullptr) {
		manageLoadCharacterError();
	}
	congelamientoTres = loadTexture(CONGELAMIENTO_NIVEL_TRES_PATH, renderer);
	if (congelamientoTres == nullptr) {
		manageLoadCharacterError();
	}
	congelamientoCuatro = loadTexture(CONGELAMIENTO_NIVEL_CUATRO_PATH,renderer);
	if (congelamientoCuatro == nullptr) {
			manageLoadCharacterError();
	}
	portal = loadTexture(portalPath, renderer);
	if (portal == nullptr) {
		manageLoadBackgroundError();
	}

	//Aca se carga la fuente
	loadFont();

	//Aca se cargan las imagenes de las figuras.
	loadMedia();

	//Aca se carga la musica
	loadMusic();

	//Play the music
	Mix_PlayMusic( gMusic, -1 );
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


void Drawer::loadMusic(){
	 //Initialize SDL_mixer con parametros default
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	//Load background music
	gMusic = Mix_LoadMUS( "resources/SoundEffects/background.mid");
	if(!gMusic){
		printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	//Load soundeffects
	gShooting = Mix_LoadWAV("resources/SoundEffects/shooting.wav");
	if(!gShooting){
		printf( "Failed to load shooting wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gJumping = Mix_LoadWAV("resources/SoundEffects/jumping.wav");
	if(!gJumping){
		printf( "Failed to load jumping wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gDying = Mix_LoadWAV("resources/SoundEffects/dying.wav");
	if(!gDying){
		printf( "Failed to load dying wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gBallBreaking = Mix_LoadWAV("resources/SoundEffects/breakball.wav");
	if(!gBallBreaking){
		printf( "Failed to load ball breaking wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gBonus = Mix_LoadWAV("resources/SoundEffects/bonus.wav");
	if(!gBonus){
		printf( "Failed to load bonus wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gOneUp = Mix_LoadWAV("resources/SoundEffects/oneup.wav");
	if(!gOneUp){
		printf( "Failed to load oneup wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gGameover = Mix_LoadWAV("resources/SoundEffects/gameover.wav");
	if(!gGameover){
		printf( "Failed to load gameover wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gTransition = Mix_LoadWAV("resources/SoundEffects/surf.wav");
	if(!gTransition){
		printf( "Failed to load gameover wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gkicking = Mix_LoadWAV("resources/SoundEffects/byebye.wav");
	if(!gkicking){
		printf( "Failed to load gameover wav! SDL_mixer Error: %s\n", Mix_GetError() );
	}
}

void Drawer::loadFont() {
	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		manageSDL_ttfError();
	}
	int sizeOfTheFont = 17;
	fontToBeUsed = TTF_OpenFont(fontPath.c_str(), sizeOfTheFont);
	if (fontToBeUsed == nullptr)
		manageSDL_ttfLoadFontError();
}

bool Drawer::loadMedia() {
	//Loading success flag
	bool success = true;

	//Load images
	if (!rectangleLT.loadFromFile(rectangleImage, renderer)) {
		printf("Failed to load rectangle texture!\n");
		success = false;
	}
	if (!icePlatformLT.loadFromFile(icePlatformImage, renderer)) {
		printf("Failed to load icePlatform texture!\n");
		success = false;
	}
	if (!circleLT.loadFromFile(circleImage, renderer)) {
		printf("Failed to load circle texture!\n");
		success = false;
	}
	if (!triangleLT.loadFromFile(triangleImagePath, renderer)) {
		printf("Failed to load triangle texture!\n");
		success = false;
	}
	if (!squareLT.loadFromFile(squareImagePath, renderer)) {
		printf("Failed to load square texture!\n");
		success = false;
	}
	if (!pentagonLT.loadFromFile(pentagonImagePath, renderer)) {
		printf("Failed to load pentagon texture!\n");
		success = false;
	}
	if (!hexagonLT.loadFromFile(hexagonImagePath, renderer)) {
		printf("Failed to load hexagon texture!\n");
		success = false;
	}
	if (!trapexLT.loadFromFile(trapexImagePath, renderer)) {
		printf("Failed to load trapex texture!\n");
		success = false;
	}
	if (!paralelogramLT.loadFromFile(paralelogramImagePath, renderer)) {
		printf("Failed to load paralelogram texture!\n");
		success = false;
	}
	if (!snowballLT.loadFromFile(snowballImagePath, renderer)) {
		printf("Failed to load snowball texture!\n");
		success = false;
	}
	if (!portalballLT.loadFromFile(portalballImagePath, renderer)) {
		printf("Failed to load portalball texture!\n");
		success = false;
	}
	if (!fireballLT.loadFromFile(fireballImagePath, renderer)) {
		printf("Failed to load fireball texture!\n");
		success = false;
	}
	if (!bonusPortalLT.loadFromFile(bonusPortalPath, renderer)) {
		printf("Failed to load bonusPortal texture!\n");
		success = false;
	}
	if (!bonusPotenciaLT.loadFromFile(bonusPotenciaPath, renderer)) {
		printf("Failed to load bonusPotencia texture!\n");
		success = false;
	}
	if (!bonusVelocidadLT.loadFromFile(bonusVelocidadPath, renderer)) {
		printf("Failed to load bonusVelocidad texture!\n");
		success = false;
	}
	if (!bonusVidaLT.loadFromFile(bonusVidaPath, renderer)) {
		printf("Failed to load bonusVida texture!\n");
		success = false;
	}

	if(!gameOverScreenLT.loadFromFile(gameOverScreenPath, renderer)){
		printf("Failed to load text texture!\n");
		success = false;
	}

	LTexture imagenBonusPortal;
	LTexture imagenBonusPotencia;
	LTexture imagenBonusVelocidad;
	LTexture imagenBonusVida;

	SDL_Color blanco = { 255, 255, 255, 0xFF };
	SDL_Color negro = { 0, 0, 0, 0xFF };

	//Load messages
	if(!pointsLT.loadFromRenderedText(renderer, fontToBeUsed, points, blanco, &anchoPoints, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!livesLT.loadFromRenderedText(renderer, fontToBeUsed, lives, blanco, &anchoLives, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!levelsLT.loadFromRenderedText(renderer, fontToBeUsed, level, negro, &anchoLevel, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}

	//Load numbers
	if(!ceroLT.loadFromRenderedText(renderer, fontToBeUsed, "0", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!unoLT.loadFromRenderedText(renderer, fontToBeUsed, "1", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!dosLT.loadFromRenderedText(renderer, fontToBeUsed, "2", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!tresLT.loadFromRenderedText(renderer, fontToBeUsed, "3", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!cuatroLT.loadFromRenderedText(renderer, fontToBeUsed, "4", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!cincoLT.loadFromRenderedText(renderer, fontToBeUsed, "5", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!seisLT.loadFromRenderedText(renderer, fontToBeUsed, "6", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!sieteLT.loadFromRenderedText(renderer, fontToBeUsed, "7", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!ochoLT.loadFromRenderedText(renderer, fontToBeUsed, "8", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!nueveLT.loadFromRenderedText(renderer, fontToBeUsed, "9", blanco, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	numerosLT = {&ceroLT, &unoLT, &dosLT, &tresLT, &cuatroLT, &cincoLT, &seisLT, &sieteLT, &ochoLT, &nueveLT};

	//Load black numbers
	if(!ceroBlackLT.loadFromRenderedText(renderer, fontToBeUsed, "0", negro, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!unoBlackLT.loadFromRenderedText(renderer, fontToBeUsed, "1", negro, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	if(!dosBlackLT.loadFromRenderedText(renderer, fontToBeUsed, "2", negro, &anchoNumber, &altoText)){
		printf("Failed to load text texture!\n");
		success = false;
	}
	numerosBlackLT = {&ceroBlackLT, &unoBlackLT, &dosBlackLT};

	loadLetters();

	return success;
}

void Drawer::loadLetters(){

	SDL_Color blanco = { 255, 255, 255, 0xFF };

	if(!aLT.loadFromRenderedText(renderer, fontToBeUsed, "a", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!bLT.loadFromRenderedText(renderer, fontToBeUsed, "b", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!cLT.loadFromRenderedText(renderer, fontToBeUsed, "c", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!dLT.loadFromRenderedText(renderer, fontToBeUsed, "d", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!eLT.loadFromRenderedText(renderer, fontToBeUsed, "e", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!fLT.loadFromRenderedText(renderer, fontToBeUsed, "f", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!gLT.loadFromRenderedText(renderer, fontToBeUsed, "g", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!hLT.loadFromRenderedText(renderer, fontToBeUsed, "h", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!iLT.loadFromRenderedText(renderer, fontToBeUsed, "i", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!jLT.loadFromRenderedText(renderer, fontToBeUsed, "j", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!kLT.loadFromRenderedText(renderer, fontToBeUsed, "k", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!lLT.loadFromRenderedText(renderer, fontToBeUsed, "l", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!mLT.loadFromRenderedText(renderer, fontToBeUsed, "m", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!nLT.loadFromRenderedText(renderer, fontToBeUsed, "n", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!oLT.loadFromRenderedText(renderer, fontToBeUsed, "o", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!pLT.loadFromRenderedText(renderer, fontToBeUsed, "p", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!qLT.loadFromRenderedText(renderer, fontToBeUsed, "q", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!rLT.loadFromRenderedText(renderer, fontToBeUsed, "r", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!sLT.loadFromRenderedText(renderer, fontToBeUsed, "s", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!tLT.loadFromRenderedText(renderer, fontToBeUsed, "t", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!uLT.loadFromRenderedText(renderer, fontToBeUsed, "u", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!vLT.loadFromRenderedText(renderer, fontToBeUsed, "v", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!wLT.loadFromRenderedText(renderer, fontToBeUsed, "w", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!xLT.loadFromRenderedText(renderer, fontToBeUsed, "x", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!yLT.loadFromRenderedText(renderer, fontToBeUsed, "y", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!zLT.loadFromRenderedText(renderer, fontToBeUsed, "z", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!ALT.loadFromRenderedText(renderer, fontToBeUsed, "A", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!BLT.loadFromRenderedText(renderer, fontToBeUsed, "B", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!CLT.loadFromRenderedText(renderer, fontToBeUsed, "C", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!DLT.loadFromRenderedText(renderer, fontToBeUsed, "D", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!ELT.loadFromRenderedText(renderer, fontToBeUsed, "E", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!FLT.loadFromRenderedText(renderer, fontToBeUsed, "F", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!GLT.loadFromRenderedText(renderer, fontToBeUsed, "G", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!HLT.loadFromRenderedText(renderer, fontToBeUsed, "H", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!ILT.loadFromRenderedText(renderer, fontToBeUsed, "I", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!JLT.loadFromRenderedText(renderer, fontToBeUsed, "J", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!KLT.loadFromRenderedText(renderer, fontToBeUsed, "K", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!LLT.loadFromRenderedText(renderer, fontToBeUsed, "L", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!MLT.loadFromRenderedText(renderer, fontToBeUsed, "M", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!NLT.loadFromRenderedText(renderer, fontToBeUsed, "N", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!OLT.loadFromRenderedText(renderer, fontToBeUsed, "O", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!PLT.loadFromRenderedText(renderer, fontToBeUsed, "P", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!QLT.loadFromRenderedText(renderer, fontToBeUsed, "Q", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!RLT.loadFromRenderedText(renderer, fontToBeUsed, "R", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!SLT.loadFromRenderedText(renderer, fontToBeUsed, "S", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!TLT.loadFromRenderedText(renderer, fontToBeUsed, "T", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!ULT.loadFromRenderedText(renderer, fontToBeUsed, "U", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!VLT.loadFromRenderedText(renderer, fontToBeUsed, "V", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!WLT.loadFromRenderedText(renderer, fontToBeUsed, "W", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!XLT.loadFromRenderedText(renderer, fontToBeUsed, "X", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!YLT.loadFromRenderedText(renderer, fontToBeUsed, "Y", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!ZLT.loadFromRenderedText(renderer, fontToBeUsed, "Z", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");
	if(!auxLT.loadFromRenderedText(renderer, fontToBeUsed, "0", blanco, &anchoNumber, &altoText))
		printf("Failed to load text texture!\n");

	letrasLT = {&ALT, &BLT, &CLT, &DLT, &ELT, &FLT, &GLT, &HLT, &ILT, &JLT, &KLT, &LLT, &MLT,
				&NLT, &OLT, &PLT, &QLT, &RLT, &SLT, &TLT, &ULT, &VLT, &WLT, &XLT, &YLT, &ZLT,
				&auxLT, &auxLT, &auxLT, &auxLT, &auxLT, &auxLT, //Estan para hacer la cuenta con la tabla ascii
				&aLT, &bLT, &cLT, &dLT, &eLT, &fLT, &gLT, &hLT, &iLT, &jLT, &kLT, &lLT, &mLT,
				&nLT, &oLT, &pLT, &qLT, &rLT, &sLT, &tLT, &uLT, &vLT, &wLT, &xLT, &yLT, &zLT};
}
