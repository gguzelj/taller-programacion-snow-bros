#include "../../headers/Exceptions/ErrorFueraDeRango.h"

const char* ErrorFueraDeRango::what() const throw(){
        return "Error en el rango de los parametros";
}
