#ifndef ERRORFUERADERANGO_H_
#define ERRORFUERADERANGO_H_

#include <exception>

class ErrorFueraDeRango: public std::exception {
public:
	virtual const char* what() const throw();
};

#endif /* ERRORFUERADERANGO_H_ */
