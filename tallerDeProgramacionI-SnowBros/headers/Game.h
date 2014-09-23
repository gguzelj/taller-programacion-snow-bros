#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include "Controlador/Controlador.h"
#include "Modelo/Escenario.h"
#include "Vista/Drawer.h"
#include "Parser/JsonParser.h"
#include "Log/Log.h"

#define GAME_OK		0
#define GAME_ERROR	1
#define GAME_RELOAD 2

#define GAME_JSON_DEF	"resources/default.json"

//MENSAJES
//ERROR
#define GAME_MSG_INIT_ERROR	"Game: Error al inicial el juego"

//WARNING
#define GAME_MSG_CANT_PARAM	"Game: La cantidad de parametros no es correcta. Se setea un archivo json default."
#define GAME_MSG_JSON_FILE	"Game: El archivo json indicado no existe. Se setea un archivo json default."

//INFO
#define GAME_MSG_NEW_GAME	"Game: Comienzo de nuevo Juego"
#define GAME_MSG_DELETE_GAME "Game: Borrando Juego"
#define GAME_MSG_RELOAD		"Game: Reinicio de Juego"
#define GAME_MSG_VAL_PAR	"Game: Validando parametros de entrada"


const float32 timeStep = 1/60.0;       //the length of time passed to simulate (seconds)
const int32 velocityIterations = 16;   //how strongly to correct velocity
const int32 positionIterations = 6;   //how strongly to correct position}

using namespace std;

class Game{
public:
	Game();
	~Game();

	/*
	 * Este es el metodo que se encarga de llevar a cabo el flujo del juego.
	 * Devuelve:
	 *  0: Sale del juego sin reiniciar
	 * 	1: Sale del juego con errores
	 * 	2: Reiniciar juego
	 */
	int onExecute(int argc, char* argv[]);

	/*
	 * El dataHandler toma la responsabilidad de inicializar el modelo y la vista de acuerdo a los datos indicados por
	 * el usuario, pasando los parametros necesarios al modelo y a la vista.
	 *
	 * Precondiciones: Debe recibir un archivo JSON.
	 * Postcondiciones: Devuelve true si salio bien, false en caso contrario.
	 */
	bool onInit(string jsonPath);

	/*
	 * Controla los eventos realizados por el usuario mediante el eventHandler
	 */
	void onEvent();

	/*
	 * Maneja las actualizaciones en el modelo (Ej: La figura x se movio dos unidades a la derecha).
	 * Sería la parte del "Modelo" de MVC
	 */
	void onLoop();

	/*
	 * Maneja aquello relacionado con lo que se tiene que dibujar en la pantalla, para luego mostrarlo.
	 * Sería la parte de "Vista" del patron MVC
	 */
	void onRender();

	/*
	 * Libera todos los recursos cargados en memoria
	 */
	void onCleanup();

private:
	bool running_;
	bool reload_;

	Controlador *controller_;
	Escenario *model_;
	Drawer *view_;
	JsonParser *parser_;
	std::string jsonPath_;

	/**
	 * Validamos los parametros recibidos por consola
	 */
	void validateParameters(int argc, char* argv[]);

};

#endif /* GAME_H_ */
