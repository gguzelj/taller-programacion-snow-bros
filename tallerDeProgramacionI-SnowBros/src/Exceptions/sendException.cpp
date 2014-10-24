#include "../../headers/Exceptions/sendException.h"

const char* sendException::what() const throw(){
        return "Error al enviar datos";
}
