#include "../headers/Client.h"
#include <iostream>

int main(int argc, char* argv[]) {

	Client* client = new Client();

	if(client->init(argc, argv) == CLIENT_ERROR){
		delete client;
		return CLIENT_ERROR;
	}

	client->run();

	delete client;

    return 0;
}
