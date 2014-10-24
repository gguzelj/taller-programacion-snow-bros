#include "../../headers/Exceptions/receiveException.h"

const char* receiveException::what() const throw(){
        return "Error al enviar datos";
}

