#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include "../Vista/Drawer.h"

#define TAM 20

typedef struct dataToSend{
	char id[TAM];
	int32_t keycode_1;
	uint32_t type_1;
	int32_t keycode_2;
	uint32_t type_2;
	int32_t keycode_3;
	uint32_t type_3;
	int32_t keycode_4;
	uint32_t type_4;
} dataToSend_t;

class Controlador{
public:
	Controlador(Drawer *view);
	~Controlador(){};

	dataToSend_t* handleEvents(bool* running);

    void handleEvent(SDL_Event* evento,bool* quit, int32_t *code, Uint32 *type);

private:
    Drawer *view;
    int cantEventos;
};



#endif /* CONTROLADOR_H_ */
