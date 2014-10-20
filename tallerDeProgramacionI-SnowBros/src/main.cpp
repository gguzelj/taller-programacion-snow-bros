#include <iostream>
#include "../headers/Server.h"
#include <thread>

// Para correr el ejecutable, pasarle los siguientes parametros:
// jsonPath: Configuracion del juego
// port: Puerto en el cual escucha el cliente
int main(int argc, char *argv[]) {

	Server *server = new Server();

	if (server->init(argc, argv) == SRV_ERROR)
		return SRV_ERROR;

	server->run();

	delete server;

	return 0;
}
