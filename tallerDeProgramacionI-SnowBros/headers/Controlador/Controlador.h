#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include "../Vista/Drawer.h"

class Controlador{
public:
	Controlador(Drawer *view);
	~Controlador(){};

    void handleEvents(bool* running, int *code, unsigned int *type);

    void handleEvent(SDL_Event* evento,bool* quit);

private:
    Drawer *view;
};



#endif /* CONTROLADOR_H_ */
