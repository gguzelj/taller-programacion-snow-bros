/*
 * JsonParser.h
 *
 *  Created on: Sep 3, 2014
 *      Author: german
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <cstring>
#include <iostream>
#include "json/json.h"

class JsonParser {
public:
	JsonParser();
	virtual ~JsonParser();

	//Implementar
	int getAlto_px();
	int getAncho_px();
	int getAlto_un();
	int getAncho_un();
	std::string getImagePath();
	int getCharacterX();
	int getCharacterY();

	void test();
};

#endif /* JSONPARSER_H_ */
