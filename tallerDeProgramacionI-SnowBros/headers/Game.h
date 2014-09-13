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
	int onExecute(string jsonPath);

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

};

#endif /* GAME_H_ */
