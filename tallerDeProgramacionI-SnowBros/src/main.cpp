#include "../headers/Client.h"
#include <iostream>

// Para correr el exe, pasarle los siguientes parametros:
// host: Representa la ip del server o bien localhost si se corre en la misma maquina
// port: EL puerto donde esta escuchando el server
// username: ID del usuario
int main(int argc, char* argv[]) {

	Client* client = new Client();

	if(client->init(argc, argv) == CLIENT_ERROR)
	{
		delete client;
		return CLIENT_ERROR;
	}

	try
	{
		client->run();
	}
	catch(...)
	{
		std::cout << "Se ha cerrado por una excepcion." << std::endl;
	}

	delete client;

    return 0;
}
