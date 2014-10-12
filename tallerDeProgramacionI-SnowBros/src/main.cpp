#include "../headers/Game.h"
#include <iostream>

int main(int argc, char* argv[]) {

	int output;
	do{
		Game* theGame = new Game();
		output = theGame->onExecute(argc, argv);
		delete theGame;
	}
	while (output == GAME_RELOAD);

    return 0;
}
