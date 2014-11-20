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

#include "Exceptions/receiveException.h"
#include "Exceptions/sendException.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <Box2D/Box2D.h>
#include "Modelo/Escenario.h"
#include "Parser/JsonParser.h"
#include "Log/Log.h"

#include "threadsafe_queue.cpp"

#define SRV_NO_ERROR 0
#define SRV_ERROR 1

#define HB_TIMEOUT 1

//Definimos los tipos de mensajes que pueden enviarse al server
#define HB_MSG_TYPE 1
#define EVENT_MSG_TYPE 2

#define TAM 20

//MENSAJES
#define SRV_MSG_CREATE					"Creando Servidor"
#define SRV_MSG_VAL_PARAM				"Validando parametros"
#define SRV_MSG_PARAM_INC				"Cantidad de parametros incorrecta"
#define SRV_MSG_PARAM_OK				"Parametros Correctos"
#define SRV_MSG_SOCK_CREATE				"Creando socket"
#define SRV_MSG_SOCK_ERROR				"Error al tratar de abrir el socket"
#define SRV_MSG_SOCK_OK					"Socket creado correctamente"
#define SRV_MSG_BIND_CREATE				"Bindeando Socket"
#define SRV_MSG_BIND_ERROR				"Error al tratar de bindear el socket"
#define SRV_MSG_BIND_OK					"Socket bindeado"
#define SRV_MSG_RUNNING_GAME			"Corriendo Juego"
#define SRV_MSG_END_GAME				"Fin del Juego"
#define SRV_MSG_CONN_MAN				"Corriendo thread para aceptar conexiones"
#define SRV_MSG_CONN_ERROR				"No se pudo aceptar la nueva conexion"
#define SRV_MSG_NEW_CLIENT				"Nuevo cliente conectado"
#define SRV_MSG_SEARCH_POS				"Buscamos lugar para la conexion. ID: "
#define SRV_MSG_ADD_CONN				"Agregamos al nuevo cliente. ID: "
#define SRV_MSG_REJ_CONN				"Capacidad maxima de clientes alcanzada. No aceptamos la conexion. ID: "
#define SRV_MSG_TRY_RECONN				"Intentamos reconectar al cliente. ID: "
#define SRV_MSG_RECONN					"Cliente reconectado. ID: "
#define SRV_MSG_RECONN_ERROR			"Ya existe una conexion con el mismo identificador. ID: "
#define SRV_MSG_INIT_DATA				"Datos iniciales del juego enviados"
#define SRV_MSG_RECEIVE_ERROR			"No se pueden recibir datos del cliente. ID: "
#define SRV_MSG_SEND_ERROR				"No se pueden enviar datos al cliente. ID: "
#define SRV_MSG_PER_MAP					"No se puede mapear el cliente con un personaje del juego. ID: "
#define SRV_MSG_NEW_PER					"Se asigno un nuevo personaje al cliente. ID: "
#define SRV_MSG_RECEIVING_THREAD		"Comienza el thread para recibir datos del cliente. ID: "
#define SRV_MSG_SEND_THREAD				"Comienza el thread para enviar datos al cliente. ID: "

typedef struct receivedData {
	char id[TAM];
	int32_t keycode_1;
	uint32_t type_1;
	int32_t keycode_2;
	uint32_t type_2;
	int32_t keycode_3;
	uint32_t type_3;
} receivedData_t;

typedef struct firstConnectionDetails {
	unsigned int nivel;
	unsigned int cantPersonajes;
	unsigned int cantEnemigos;
	unsigned int cantObjDinamicos;
	unsigned int cantObjEstaticos;
} firstConnectionDetails_t;

typedef struct dataToSend {
	gameData_t* gameData;
	personaje_t* personajes;
	enemigo_t* enemigos;
	figura_t* dinamicos;
	proyectil_t* proyectiles;
} dataToSend_t;

typedef char conn_id[TAM];

typedef struct connection {
	bool activa;
	conn_id id;
	int socket;
	Threadsafe_queue<dataToSend_t>* dataQueue;
} connection_t;

/*
 * Clase Server
 */
class Server {
public:
	Server();
	virtual ~Server();

	/*
	 * Metodo para iniciar el servidor creando el socket y bindeandolo
	 */
	int init(int argc, char *argv[]);

	/*
	 * Metodo que se encarga de la ejecucion del servidor
	 */
	void run();

private:

	bool running_;
	int port_;
	int sockfd_;
	unsigned int connectionsLimit_;
	bool acceptNewClients_;
	firstConnectionDetails_t datos_;
	gameData_t gameData_;

	Escenario *model_;
	JsonParser *parser_;
	std::string jsonPath_;

	std::thread newConnectionsThread_;

	std::vector<connection_t*> connections_;
	Threadsafe_queue<receivedData_t*>* shared_rcv_queue_;

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

	/*
	 * Metodo encargado de negociar la conexion con el cliente
	 */
	int acceptConnection(int newsockfd);


	void pasarDeNivel();


	/*
	 * Manejamos el caso de un nuevo cliente
	 */
	int manejarNuevoCliente(connection_t *conn);

	/*
	 * Manejamos el caso de una reconexion
	 */
	int manejarReconexion(connection_t *conn);

	/**
	 * Validamos si con la ultima conexion el juego puede comenzar
	 */
	int validarComienzoJuego();

	/*
	 * Metodo utilizado para enviar por primera vez todos los datos del juego
	 * a un nuevo cliente
	 */
	void enviarDatosJuego(int sockfd);

	/*
	 * Metodo encargado de crearle el personaje correspondiente al usuario que se acaba de conectar, referenciado
	 * por el parametro connection
	 */
	void crearPersonaje(connection_t* connection, bool reconexion);

	/*
	 * Determina si el usario conectado es nuevo (return true) o bien ya tiene un historial en el juego (return false)
	 */
	bool esNuevoCliente(conn_id id);

	/*
	 * Lanza el thread para que el cliente pueda empezar a mandar eventos en forma paralela
	 */
	void initReceivingThread(connection_t* connection);

	/*
	 * Crea la queue para el envio de datos del server al cliente y luego lanza el thread
	 * para que el server ya pueda mandarle info en forma paralela
	 */
	void initSendingThread(connection_t* connection);

	//////////////////////////////////
	//Thread de recepcion de eventos//
	//////////////////////////////////
	/*
	 * Metodo que se encarga de encolar los eventos enviados por el cliente en la shared_rcv_queue.
	 * En caso de detectar que el cliente se desconecto, lanzamos excepcion
	 */
	void recibirDelCliente(connection_t *conn);

	/*
	 * Metodo de bajo nivel de sockets para recibir hasta una cierta
	 * cantidad de bytes determinada.
	 */
	void recvall(int s, void *data, int len) throw (receiveException);

	///////////////////////////
	//Thread de envio de info//
	///////////////////////////

	/*
	 * Metodo encargado de enviarle al cliente los datos del modelo, encolados en la personal_queue
	 */
	void enviarAlCliente(connection_t *conn);

	/*
	 * Toma la informacion necesario del modelo para generar un struct que se enviara a los clientes.
	 * Una vez generado el struct lo encola en una cola de envios, la cual sera utilizada para enviarle a los threads
	 * de envio, el dato a mandar
	 */
	void enviarAClientes();

	/*
	 * Metodo para enviar proyectiles al cliente
	 */
	void enviarProyectiles(int sock, proyectil_t* dinamicos);

	/*
	 * Metodo para enviar los objetos dinamicos
	 */
	void enviarDinamicos(int sock, figura_t* dinamicos);

	/*
	 * Metodo para enviar los objetos estaticos
	 */
	void enviarEstaticos(int sock, figura_t* estaticos);

	/*
	 * Enviamos informacion del juego
	 */
	void enviarGameData(int sock, gameData_t* gameData);

	/*
	 * Metodo para enviar los personajes
	 */
	void enviarPersonajes(int sock, personaje_t* personajes);

	/*
	 * Metodo para enviar los enemigos
	 */
	void enviarEnemigos(int sock, enemigo_t* enemigos);

	/*
	 * Metodo de bajo nivel de sockets para enviar hasta una cierta
	 * cantidad de bytes determinada.
	 */
	void sendall(int s, void *data, int len) throw (sendException);

	////////////////////////////////////////////////////////////////////////////
	//Thread principal. Se comunican con los otros threads mediante las queues//
	////////////////////////////////////////////////////////////////////////////
	void step();

	/*
	 * Metodo para crear el socket que va a utilizar el server
	 * Devuelve 0 si no hubo error, 1 en otro caso
	 */
	int createSocket();

	/*
	 * Metodo para bindear el socket creado.
	 * Devuelve 0 si no hubo error, 1 en otro caso
	 */
	int bindSocket();

	/*
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
