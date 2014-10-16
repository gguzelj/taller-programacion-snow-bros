#include <iostream>
#include "../headers/Server.h"
#include <thread>

int main(int argc, char *argv[]) {

	if (argc < 2) {
		std::cout << "ERROR, no port provided\n" << std::endl;
		exit(1);
	}

	Server *server = new Server(argv[1]);
	server->init();
	server->run();

	delete server;

	return 0;
}
