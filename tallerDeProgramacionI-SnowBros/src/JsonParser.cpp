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

	//EL archivo ya deberia estar cargado por alguna otra clase. Esto hay que borrarlo!
	std::ifstream t(
			"/home/german/Desktop/taller-programacion-snow-bros/tallerDeProgramacionI-SnowBros/resources/example.json");
	std::string str;

	if (t.is_open()) {
		t.seekg(0, std::ios::end);
		jsonFile_.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		jsonFile_.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
	}

	//El metodo empezaria aca
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

	bool parsedSuccess;

	Json::Reader reader;

	Json::Value root;
	Json::Value escenario;
	Json::Value alto_px;
	Json::Value ancho_px;
	Json::Value alto_un;
	Json::Value ancho_un;
	Json::Value imagen_fondo;
	Json::Value personaje;
	Json::Value x;
	Json::Value y;
	Json::Value objetos;
	Json::Value objeto;
	Json::Value tipo;
	Json::Value ancho;
	Json::Value alto;
	Json::Value color;
	Json::Value rot;
	Json::Value masa;
	Json::Value estatico;

	//Leemos el archivo
	parsedSuccess = reader.parse(jsonFile_, root, false);

	if (not parsedSuccess) {
		//ERROR, agregar logica para el manero de errores
		return;
	}

	escenario = root["escenario"]; //Cambiar por cte

	alto_px = escenario.get("alto-px", 2); //Cambiar por cte
	ancho_px = escenario.get("ancho-px", 2); //Cambiar por cte
	alto_un = escenario.get("alto-un", 2); //Cambiar por cte
	ancho_un = escenario.get("ancho-un", 2); //Cambiar por cte
	imagen_fondo = escenario.get("imagen_fondo", 2); //Cambiar por cte
	personaje = escenario.get("personaje", 2); //Cambiar por cte

	x = personaje.get("x", 2); //Cambiar por cte
	y = personaje.get("y", 2); //Cambiar por cte

	objetos = escenario["objetos"];

	for (unsigned int i = 0; i < objetos.size(); ++i) {
		tipo = objetos[i].get("tipo", "rect");
		x = objetos[i].get("x", 5);
		y = objetos[i].get("y", 90);
		ancho = objetos[i].get("ancho", 2);
		alto = objetos[i].get("alto", 1);
		color = objetos[i].get("color", "#00FF00");
		rot = objetos[i].get("rot", 45);
		masa = objetos[i].get("masa", 3);
		estatico = objetos[i].get("estatico", false);
	}

	std::cout << rot.asInt() << std::endl;
	return;
}
