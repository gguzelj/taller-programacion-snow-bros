#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include "../Vista/Drawer.h"

typedef struct dataToSend{
	int keycode_1;		//TODO en realidad son Sint32 y Uint32 pero no me los toma, despues checkear.
	unsigned int type_1;
	int keycode_2;
	unsigned int type_2;
} dataToSend_t;

class Controlador{
public:
	Controlador(Drawer *view);
	~Controlador(){};

    void handleEvents(bool* running, dataToSend_t* data);

    void handleEvent(SDL_Event* evento,bool* quit, int *code, unsigned int *type);

private:
    Drawer *view;
    int cantEventos;
};



#endif /* CONTROLADOR_H_ */
