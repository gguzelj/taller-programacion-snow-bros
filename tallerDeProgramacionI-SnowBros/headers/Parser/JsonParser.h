#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

#include <Box2D/Box2D.h>
#include <json/json.h>

#include "JsonParser_C.h"
#include "ParserValidator.h"
#include "FileReader.h"

class JsonParser {

public:
	JsonParser();
	JsonParser(std::string JsonFile);
	virtual ~JsonParser();

	void parse();

	//Metodos utilizados por el modelo
	int getAltoPx();
	int getAnchoPx();
	int getAltoUn();
	int getAnchoUn();
	std::list<b2BodyDef> getObjetos();

	//Metodos utilizados por la vista
	std::string getImagenFondo();

private:

	Json::Reader reader_;

	Json::Value root_;
	Json::Value alto_px_;
	Json::Value ancho_px_;
	Json::Value alto_un_;
	Json::Value ancho_un_;
	Json::Value imagen_fondo_;
	Json::Value personaje_;
	Json::Value x_;
	Json::Value y_;
	Json::Value objetosEscenario_;
	Json::Value objeto_;
	Json::Value tipo_;
	Json::Value ancho_;
	Json::Value alto_;
	Json::Value color_;
	Json::Value rot_;
	Json::Value masa_;
	Json::Value estatico_;

	std::string jsonFile_;
	std::list<b2BodyDef> objetos_;

	void setDefaultValues();
	void setValuesFromFile();
	void parseEscenario(Json::Value escenario);

};

#endif /* JSONPARSER_H_ */
