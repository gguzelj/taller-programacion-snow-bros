#include "../../headers/Exceptions/ErrorTipoDeObjeto.h"

const char* ErrorTipoDeObjeto::what() const throw(){
        return "El tipo de objeto requerido no es valido. Observar el archivo de log para mayor informacion";
}

