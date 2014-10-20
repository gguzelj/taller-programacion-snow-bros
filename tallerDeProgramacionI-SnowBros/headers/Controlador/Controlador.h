#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include "../Vista/Drawer.h"

#define TAM 20

typedef struct dataToSend{
	char id[TAM];
	int32_t keycode_1;		//TODO en realidad son Sint32 y Uint32 pero no me los toma, despues checkear.
	uint32_t type_1;
	int32_t keycode_2;
	uint32_t type_2;
} dataToSend_t;

class Controlador{
public:
	Controlador(Drawer *view);
	~Controlador(){};

	dataToSend_t* handleEvents(bool* running);

    void handleEvent(SDL_Event* evento,bool* quit, int *code, unsigned int *type);

private:
    Drawer *view;
    int cantEventos;
};



#endif /* CONTROLADOR_H_ */
