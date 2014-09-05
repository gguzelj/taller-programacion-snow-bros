#ifndef GESTORDEEVENTOS_H_
#define GESTORDEEVENTOS_H_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <sstream>
#include <iostream>
#include "constants.h"
#include "../headers/constants.h"

class GestorDeEventos {
public:
        GestorDeEventos();
        virtual ~GestorDeEventos();

        void listenEvents(SDL_Event* event, bool* running, bool* reload);

        void interpretarEvento(SDL_Event* evento,bool* quit, bool* reload);

};


#endif /* GESTORDEEVENTOS_H_ */
