#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <json/json.h>

#include "JsonParser_C.h"
#include "FileReader.h"

class JsonParser {

public:
	JsonParser();
	JsonParser(std::string JsonFile);
	virtual ~JsonParser();

	void setJsonFile(std::string jsonFile);
	std::string getJsonFile();

	void parse();

private:
	std::string jsonFile_;


	void setDefaultValues();
	void setValuesFromFile();

};

#endif /* JSONPARSER_H_ */
