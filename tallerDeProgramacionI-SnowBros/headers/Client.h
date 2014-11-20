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
#include <unistd.h>

#include "threadsafe_queue.cpp"
#include "Exceptions/receiveException.h"
#include "Exceptions/sendException.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Controlador/Controlador.h"
#include "Vista/Drawer.h"
#include "Log/Log.h"

#define CLIENT_OK		0
#define CLIENT_ERROR	1

#define SRV_NO_ERROR 0
#define SRV_ERROR 1

#define HB_TIMEOUT 1

//Definimos los tipos de mensajes que pueden enviarse al server
#define HB_MSG_TYPE 1
#define EVENT_MSG_TYPE 2

//MENSAJES
//ERROR
#define CLIENT_MSG_INIT_ERROR			"Client: Error al iniciar el juego"
#define CLIENT_MSG_INVALID_PORT_ERROR 	"Client: El puerto no es correcto"
#define CLIENT_MSG_CANT_PARAM			"Client: La cantidad de parametros no es correcta."
#define CLIENT_MSG_ERROR_WHEN_RECEIVING	"Client: Error al utilizar recvall(), problemas en la conexion?"
#define CLIENT_MSG_ERROR_WHEN_SENDING	"Client: Error al utilizar sendall(), problemas en el servidor?"
#define CLIENT_MSG_SOCK					"Client: No se pudo crear el socket"
#define CLIENT_MSG_SOCK_TIMEOUT			"Client: No se pudo setear un timeout para el socket"

//INFO
#define CLIENT_MSG_NEW_CLIENT			"Client: Comienzo de nuevo Juego"
#define CLIENT_MSG_DELETE_CLIENT 		"Client: Borrando Juego"
#define CLIENT_MSG_VAL_PAR				"Client: Validando parametros de entrada"

using namespace std;

//Estructuras necesarias para conectarse con el servidor
typedef struct firstConnectionDetails{
	unsigned int nivel;
	unsigned int cantPersonajes;
	unsigned int cantEnemigos;
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
} firstConnectionDetails_t;



typedef struct dataFromServer{
	gameData_t* gameData;
	personaje_t* personajes;
	enemigo_t* enemigos;
	figura_t* dinamicos;
	proyectil_t* proyectiles;
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

	std::thread hbTh;
	std::thread sendTh;
	std::thread recvTh;

    Threadsafe_queue<dataFromServer_t>* shared_rcv_queue_;

	Controlador *controller_;
	Drawer *view_;
	firstConnectionDetails_t gameDetails_;
	figura_t* estaticos_;
	figura_t* dinamicos_;
	personaje_t* personajes_;
	enemigo_t* enemigos_;

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
	 * Metodo para enviar heartBeats al servidor
	 */
	void enviarHeartBeat();

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
	int validateParameters(int argc, char* argv[]);

	/*
	 * Metodo para recibir los objetos dinamicos
	 */
	void recibirDinamicos(figura_t* &dinamicos)throw (receiveException);

	/*
	 * Recibimos los proyectiles del juego
	 */
	void recibirProyectiles(proyectil_t* &proyectiles, unsigned int cant)throw (receiveException);

	/*
	 * Metodo para recibir los objetos estaticos
	 */
	void recibirEstaticos(figura_t* &estaticos)throw (receiveException);

	/*
	 * Metodo para recibir los personajes
	 */
	void recibirPersonajes(personaje_t* &personajes)throw (receiveException);

	/*
	 * Metodo para recibir los enemigos
	 */
	void recibirEnemigos(enemigo_t* &enemigos, unsigned int cant)throw (receiveException);

	/*
	 * Metodo para recibir info general del juego
	 */
	void recibirGameData(gameData_t* &gameData)throw (receiveException);

	/*
     * Metodo de bajo nivel de sockets para recibir hasta una cierta
     * cantidad de bytes determinada.
     */
	void recvall(int s, void *data, int len) throw(receiveException);

    /*
     * Metodo de bajo nivel de sockets para enviar hasta una cierta
     * cantidad de bytes determinada.
     */
	void sendall(int s, void *data, int len) throw(sendException);

};

#endif /* CLIENT_H_ */
