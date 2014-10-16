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

#include "threadsafe_queue.cpp"

#define NO_ERROR 0
#define ERROR 1
#define BACKLOG 10 // how many pending connections queue will hold
#define MICROSECONDS 5000000
#define COTA_ESPERA 10000

typedef struct punto{
	float x;
	float y;
} punto_t;

typedef struct receivedData{
	std::string username;
	SDL_Event event;
} receivedData_t;

typedef struct dataToSend{
	std::string object_id;
	punto_t centro;
	float ancho;
	float alto;
	float rotacion;
} dataToSend_t;

class Server {
public:
	Server(char *port);
	virtual ~Server();

	//Metodo para iniciar el servidor creando el socket y bindeandolo
	int init();

	//Metodo que se encarga de la ejecucion del servidor
	void run();

private:
	bool running_;
	int port_;
	int sockfd_;
	bool acceptNewClients_;

	std::thread newConnectionsThread_;

	std::vector<int> sockets_;
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

	//////////////////////////////////
	//Thread de recepcion de eventos//
	//////////////////////////////////
	/*
	 * Metodo que se encarga de encolar los eventos enviados por el cliente en la shared_rcv_queue.
	 * En caso de detectar que el cliente se desconecto, lanzamos excepcion
	 */
	void recibirDelCliente(int sock, Threadsafe_queue<receivedData_t*>* shared_rcv_queue);
	/*
	 * Metodo de bajo nivel de sockets
	 */
	int recvall(int s, receivedData_t *data, int *len);

	///////////////////////////
	//Thread de envio de info//
	///////////////////////////

	//Metodo encargado de enviarle al cliente los datos del modelo, encolados en la personal_queue
	void enviarAlCliente(int sock, Threadsafe_queue<dataToSend_t>* personal_queue);

	//Metodo de bajo nivel de sockets
	int sendall(int s, dataToSend_t *data, int *len);

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
	 * Metodo para bindear el socket creado.
	 * Devuelve 0 si no hubo error, 1 en otro caso
	 */
	int bindSocket();
};

#endif /* SERVER_H_ */
