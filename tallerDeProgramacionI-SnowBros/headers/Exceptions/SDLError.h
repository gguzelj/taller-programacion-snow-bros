#ifndef SDLERROR_H_
#define SDLERROR_H_

//using standard exceptions
#include <iostream>
#include <exception>
using namespace std;

class SDLError: public exception{
public:
        virtual const char* what() const throw();
};

#endif /* SDLERROR_H_ */
