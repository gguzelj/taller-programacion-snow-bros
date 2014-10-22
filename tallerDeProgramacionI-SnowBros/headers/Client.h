#ifndef CLIENT_H_
#define CLIENT_H_

#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "threadsafe_queue.cpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Controlador/Controlador.h"
#include "Vista/Drawer.h"
#include "Log/Log.h"

#define CLIENT_OK		0
#define CLIENT_ERROR	1

#define SRV_NO_ERROR 0
#define SRV_ERROR 1

//MENSAJES
//ERROR
#define CLIENT_MSG_INIT_ERROR			"Client: Error al iniciar el juego"
#define CLIENT_MSG_INVALID_PORT_ERROR 	"Client: El puerto no es correcto"
#define CLIENT_MSG_CANT_PARAM			"Client: La cantidad de parametros no es correcta."
#define CLIENT_MSG_ERROR_WHEN_RECEIVING	"Client: Error al utilizar recvall(), problemas en la conexion?"
#define CLIENT_MSG_ERROR_WHEN_SENDING	"Client: Error al utilizar sendall(), problemas en el servidor?"

//INFO
#define CLIENT_MSG_NEW_CLIENT			"Client: Comienzo de nuevo Juego"
#define CLIENT_MSG_DELETE_CLIENT 		"Client: Borrando Juego"
#define CLIENT_MSG_VAL_PAR				"Client: Validando parametros de entrada"

using namespace std;

//Estructuras necesarias para conectarse con el servidor
typedef struct firstConnectionDetails{
	unsigned int cantPersonajes;
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
} firstConnectionDetails_t;

typedef struct dataFromServer{
	personaje_t* personajes;
	figura_t* dinamicos;
} dataFromServer_t;

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

private:
	bool running_;

	int port;
	int sock;
	const char* host;
	char* name;

	std::thread sendTh;
	std::thread recvTh;

    Threadsafe_queue<dataFromServer_t>* shared_rcv_queue_;

	Controlador *controller_;
	Drawer *view_;
	firstConnectionDetails_t gameDetails_;
	figura_t* estaticos_;
	figura_t* dinamicos_;
	personaje_t* personajes_;

	/**
	 * Metodo para crear el socket con el que va a trabajar el server
	 */
	int createSocket();

	/*
	 * Metodo para iniciar la conexion con el servidor.
	 */
	int connectToServer();

	/*
	 * Metodo que manda el ID del cliente al servidor y recibe la informacion del mundo.
	 */
	int initialize();

	/*
	 * Controla los eventos realizados por el usuario mediante un handleEvent. Recibe por parametro
	 * punteros a los miembros de un dataToSend_t y los modifica segun el
	 * evento activado por el usuario.
	 */
	dataToSend_t* onEvent();

	/*
	 * Maneja aquello relacionado con lo que se tiene que dibujar en la pantalla, para luego mostrarlo.
	 * Sería la parte de "Vista" del patron MVC
	 */
	void onRender(dataFromServer data);

	/*
	 * Libera todos los recursos cargados en memoria
	 */
	void onCleanup();

	/*
	 * Metodo en donde estara corriendo el thread que envia informacion al servidor.
	 */
	void enviarAlServer();

	/*
	 * Metodo en donde estara corriendo el thread que recibe informacion del servidor.
	 */
	void recibirDelServer();

	/*
	 * Validamos los parametros recibidos por consola
	 */
	bool validateParameters(int argc, char* argv[]);

    /*
     * Metodo de bajo nivel de sockets para recibir hasta una cierta
     * cantidad de bytes determinada.
     */
	int recvall(int s, void *data, int *len);

    /*
     * Metodo de bajo nivel de sockets para enviar hasta una cierta
     * cantidad de bytes determinada.
     */
	int sendall(int s, void *data, int *len);

};

#endif /* CLIENT_H_ */