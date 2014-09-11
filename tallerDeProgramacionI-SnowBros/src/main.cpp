#include "../headers/Game.h"
#include <iostream>

int main(int argc, char* argv[]) {

	return 0;
	int output = 0;
	do{
		Game* theGame = new Game();
		std::string jsonPath = "resources/example.json";
		output = theGame->onExecute(jsonPath);
		delete theGame;
	}
	while (output == 2); //Si la salida es 2, resetea

    return 0;
}
