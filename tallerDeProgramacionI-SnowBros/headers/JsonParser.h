/*
 * JsonParser.h
 *
 *  Created on: Sep 3, 2014
 *      Author: german
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <json/json.h>

class JsonParser {

public:
	JsonParser();
	JsonParser(std::string JsonFile);
	virtual ~JsonParser();

	void setJsonFile(std::string jsonFile);
	std::string getJsonFile();

	void parse();

	void test();

private:
	std::string jsonFile_;


	void setDefaultValues();
	void setValuesFromFile();

};

#endif /* JSONPARSER_H_ */
