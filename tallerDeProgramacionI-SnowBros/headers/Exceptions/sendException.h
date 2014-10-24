#ifndef SENDEXCEPTION_H_
#define SENDEXCEPTION_H_

#include <exception>

class sendException: public std::exception {
public:
	virtual const char* what() const throw();
};

#endif /* SENDEXCEPTION_H_ */
