#ifndef ERRORTIPODEOBJETO_H_
#define ERRORTIPODEOBJETO_H_

//using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class ErrorTipoDeObjeto: public exception{
public:
        virtual const char* what() const throw();
};

#endif /* ERRORTIPODEOBJETO_H_ */
