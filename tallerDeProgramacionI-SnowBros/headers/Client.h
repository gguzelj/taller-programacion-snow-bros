#ifndef CLIENT_H_
#define CLIENT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Controlador/Controlador.h"
#include "Vista/Drawer.h"
#include "Log/Log.h"

#define CLIENT_OK		0
#define CLIENT_ERROR	1

//MENSAJES
//ERROR
#define CLIENT_MSG_INIT_ERROR	"Client: Error al inicial el juego"
#define CLIENT_MSG_INVALID_PORT_ERROR "Client: El puerto no es correcto"
#define CLIENT_MSG_CANT_PARAM	"Client: La cantidad de parametros no es correcta."

//INFO
#define CLIENT_MSG_NEW_CLIENT	"Client: Comienzo de nuevo Juego"
#define CLIENT_MSG_DELETE_CLIENT "Client: Borrando Juego"
#define CLIENT_MSG_VAL_PAR	"Client: Validando parametros de entrada"


const float32 timeStep = 1/60.0;       //the length of time passed to simulate (seconds)
const int32 velocityIterations = 32;   //how strongly to correct velocity
const int32 positionIterations = 64;   //how strongly to correct position}

using namespace std;

class Client{
public:
	Client();
	~Client();

	/*
	 * El dataHandler toma la responsabilidad de inicializar el modelo y la vista de acuerdo a los datos indicados por
	 * el usuario, pasando los parametros necesarios al modelo y a la vista.
	 *
	 * Precondiciones: Debe recibir un archivo JSON.
	 * Postcondiciones: Devuelve true si salio bien, false en caso contrario.
	 */
	bool init(int argc, char* argv[]);

	/*
	 * Este es el metodo que se encarga de llevar a cabo el flujo del juego.
	 * Devuelve:
	 *  0: Sale del juego sin reiniciar
	 * 	1: Sale del juego con errores
	 * 	2: Reiniciar juego
	 */
	int run();


	/*
	 * Controla los eventos realizados por el usuario mediante el eventHandler
	 */
	void onEvent();

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

	Controlador *controller_;
	Drawer *view_;
	std::string host;
	int port;
	std::string name;

	/**
	 * Validamos los parametros recibidos por consola
	 */
	bool validateParameters(int argc, char* argv[]);

};

#endif /* CLIENT_H_ */
