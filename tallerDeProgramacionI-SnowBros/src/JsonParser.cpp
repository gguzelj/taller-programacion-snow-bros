/*
 * JsonParser.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: german
 */

#include "../headers/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser() {
	return;
}

JsonParser::JsonParser(std::string jsonFile) {
	jsonFile_ = jsonFile;
}

/**
 * Destructor
 */
JsonParser::~JsonParser() {
}

/**
 * Set jsonFile
 */
void JsonParser::setJsonFile(std::string jsonFile) {
	jsonFile_ = jsonFile;
}

/**
 * Get jsonFile
 */
std::string JsonParser::getJsonFile() {
	return jsonFile_;
}

/**
 * Realizamos el parseo del archivo
 */
void JsonParser::parse() {

	std::ifstream t("/home/german/Desktop/taller-programacion-snow-bros/tallerDeProgramacionI-SnowBros/resources/example.json");
	std::string str;

	if (t.is_open()) {
		t.seekg(0, std::ios::end);
		jsonFile_.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		jsonFile_.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

	}

	if (jsonFile_.empty())
		this->setDefaultValues();
	else
		this->setValuesFromFile();

	return;
}

/**
 * En los casos donde no se haya seteado ningun archivo json, cargamos juego
 * con los valores por default
 */
void JsonParser::setDefaultValues() {
	//Logica para cargar vlores por default
}

/**
 * En los casos donde no se haya asignado un algun archivo json, cargamos los valores
 * del mismo
 */
void JsonParser::setValuesFromFile() {

	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(jsonFile_, root, false);

	if (not parsedSuccess) {
		//ERROR, agregar logica para el manero de errores
		return;
	}

	const Json::Value escenario = root["escenario"]; //Cambiar por cte

	Json::Value alto_px = escenario["alto-px"]; //Cambiar por cte

	std::cout << alto_px.asString() << std::endl;

	return;

}

/**
 * Test
 */
void JsonParser::test() {

	std::string json_example =
			"{\"array\": \
	                            [\"item1\", \
	                            \"item2\"], \
	                            \"not an array\": \
	                            \"asdf\" \
	                         }";

	// Let's parse it
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_example, root, false);

	if (not parsedSuccess) {
		// Report failures and their locations
		// in the document.
		std::cout << "Failed to parse JSON" << std::endl
				<< reader.getFormatedErrorMessages() << std::endl;
		return;
	}

	// Let's extract the array contained
	// in the root object
	const Json::Value array = root["array"];

	// Iterate over sequence elements and
	// print its values
	for (unsigned int index = 0; index < array.size(); ++index) {
		std::cout << "Element " << index << " in array: "
				<< array[index].asString() << std::endl;
	}

	// Lets extract the not array element
	// contained in the root object and
	// print its value
	const Json::Value notAnArray = root["not an array"];

	if (not notAnArray.isNull()) {
		std::cout << "Not an array: " << notAnArray.asString() << std::endl;
	}

	// If we want to print JSON is as easy as doing:
	std::cout << "Json Example pretty print: " << std::endl
			<< root.toStyledString() << std::endl;

	return;

}
