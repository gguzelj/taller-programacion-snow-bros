#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include <Box2D/Box2D.h>
#include "../Modelo/Escenario.h"
#include "../Vista/Drawer.h"

#define TECLA_RESET 114

class Controlador{
public:
	Controlador(Escenario *model, Drawer *view);
	~Controlador(){};

    void handleEvents(bool* running, bool* reload);

    void handleEvent(SDL_Event* evento,bool* quit, bool* reload);

private:
    Escenario *model;
    Drawer *view;
    int presionando;
    int movimientoTipo;

};



#endif /* CONTROLADOR_H_ */
