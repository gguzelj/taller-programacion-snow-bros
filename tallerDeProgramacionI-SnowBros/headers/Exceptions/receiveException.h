/*
 * receiveException.h
 *
 *  Created on: Oct 24, 2014
 *      Author: german
 */

#ifndef RECEIVEEXCEPTION_H_
#define RECEIVEEXCEPTION_H_

#include <exception>

class receiveException: public std::exception {
public:
	virtual const char* what() const throw();
};

#endif /* RECEIVEEXCEPTION_H_ */
