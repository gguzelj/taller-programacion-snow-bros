#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <future>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include "Modelo/Escenario.h"
#include "Parser/JsonParser.h"
#include "Log/Log.h"

#include "threadsafe_queue.cpp"

#define SRV_NO_ERROR 0
#define SRV_ERROR 1
#define BACKLOG 10 // how many pending connections queue will hold
#define MICROSECONDS 5000000
#define COTA_ESPERA 10000

typedef struct receivedData{
	conn_id id;
	int keycode_1;
	int keycode_2;
	unsigned int type_1;
	unsigned int type_2;
} receivedData_t;

typedef struct firstConnectionDetails{
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
} firstConnectionDetails_t;

typedef struct dataToSend{
	std::string object_id;
	punto_t centro;
	float ancho;
	float alto;
	float rotacion;
} dataToSend_t;

typedef char conn_id[10];

typedef struct connection{
	bool activa;
	conn_id id;
	int socket;
} connection_t;


/**
 * Clase Server
 */
class Server {
public:
	Server();
	virtual ~Server();

	//Metodo para iniciar el servidor creando el socket y bindeandolo
	int init(int argc, char *argv[]);

	//Metodo que se encarga de la ejecucion del servidor
	void run();

private:

	bool running_;
	int port_;
	int sockfd_;
	bool acceptNewClients_;

	Escenario *model_;
	JsonParser *parser_;
	std::string jsonPath_;

	std::thread newConnectionsThread_;

	std::vector<connection_t> connections_;
	std::vector<std::thread> rcv_threads_;
	std::vector<std::thread> snd_threads_;
	Threadsafe_queue<receivedData_t*>* shared_rcv_queue_;
	std::vector<Threadsafe_queue<dataToSend_t>*> per_thread_snd_queues_;	//No se toman referencias ya que es info que va a ser transmitida

	//////////////////////////////////////////
	//Thread para inicializar las conexiones//
	//////////////////////////////////////////
	/*
	 * Metodo que se encarga de inicializar una conexion con el cliente.
	 * SI el cliente es aceptado, lanza los threads de enviar y recibir datos, correspondientes al nuevo cliente. A su vez
	 * se guarda dicho socket en un vector de sockets
	 * Este metodo ademas seria el encargado de autenticar a los clientes y mandarle la info inicial del juego
	 */
	void newConnectionsManager();

	/**
	 * Metodo encargado de negociar la conexion con el cliente
	 */
	int acceptConnection (int newsockfd);

	/**
	 * Buscamos si existe lugar disponible para una nueva conexion
	 */
	bool searchPlaceForConnection(connection_t conn, unsigned int &index);

	/**
	 * Metodo utilizado para enviar por primera vez todos los datos del juego
	 * a un nuevo cliente
	 */
	void enviarDatosJuego(int sockfd);

	//////////////////////////////////
	//Thread de recepcion de eventos//
	//////////////////////////////////
	/*
	 * Metodo que se encarga de encolar los eventos enviados por el cliente en la shared_rcv_queue.
	 * En caso de detectar que el cliente se desconecto, lanzamos excepcion
	 */
	void recibirDelCliente(connection_t conn);

	/*
	 * Metodo de bajo nivel de sockets
	 */
	int recvall(int s, void* data, int* len);

	///////////////////////////
	//Thread de envio de info//
	///////////////////////////

	/**
	 * Metodo encargado de enviarle al cliente los datos del modelo, encolados en la personal_queue
	 */
	void enviarAlCliente(connection_t conn, Threadsafe_queue<dataToSend_t>* personal_queue);

	/*
	 * Metodo de bajo nivel de sockets
	 */
	int sendall(int s, void* data, int* len);

	////////////////////////////////////////////////////////////////////////////
	//Thread principal. Se comunican con los otros threads mediante las queues//
	////////////////////////////////////////////////////////////////////////////
	void step();
	/*
	 * Toma la informacion necesario del modelo para generar un struct que se enviara a los clientes.
	 * Una vez generado el struct lo encola en una cola de envios, la cual sera utilizada para enviarle a los threads
	 * de envio, el dato a mandar
	 */
	void prepararEnvio();

	/**
	 * Metodo para crear el socket que va a utilizar el server
	 * Devuelve 0 si no hubo error, 1 en otro caso
	 */
	int createSocket();

	/**
	 * Metodo para bindear el socket creado.
	 * Devuelve 0 si no hubo error, 1 en otro caso
	 */
	int bindSocket();

	/**
	 * Metodo para Validar parametros con los que se ejecuta el server
	 */
	int validateParameters(int argc, char *argv[]);

	/*
	 * Seleccionan aleatoriamente las coordenadas iniciales de los personajes, evitando que se interpongan con otros
	 */
	float getInitialX();
	float getInitialY();
};

#endif /* SERVER_H_ */
