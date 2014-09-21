#include "../../headers/Exceptions/SDLError.h"

const char* SDLError::what() const throw(){
        return "Ha ocurrido un error en la inicializacion de SDL. Observar el archivo de log para mayor informacion";
}
