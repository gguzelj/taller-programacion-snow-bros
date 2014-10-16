#include <iostream>
#include "../headers/Server.h"
#include <thread>

int main(int argc, char *argv[]) {

	Server *server = new Server();

	if (server->init(argc, argv) == SRV_ERROR)
		return SRV_ERROR;

	server->run();

	delete server;

	return 0;
}
