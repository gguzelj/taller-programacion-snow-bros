#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

#include <Box2D/Box2D.h>
#include <json/json.h>

#include "../Log/Log.h"
#include "JsonParser_C.h"
#include "ParserValidator.h"
#include "FileReader.h"

class JsonParser {

public:

	//Metodos
	JsonParser(std::string JsonFile);
	virtual ~JsonParser();

	bool parse();

	unsigned int getCantidadObjetos();

	//Metodos utilizados por el modelo
	float 		getAltoUnEscenario(){ return escenario_->altoUn;};
	float 		getAnchoUnEscenario(){ return escenario_->anchoUn;};
	float 		getCoorXPersonaje(){ return personaje_->x;};
	float 		getCoorYPersonaje(){ return personaje_->y;};
	std::string getTipoObjeto(unsigned int index);
	float 		getCoorXObjeto(unsigned int index);
	float 		getCoorYObjeto(unsigned int index);
	float		getEscalaObjeto(unsigned int index);
	int			getLadosObjeto(unsigned int index);
	float 		getAnchoObjeto(unsigned int index);
	float		getAltoObjeto(unsigned int index);
	std::string getColorObjeto(unsigned int index);
	int 		getRotObjeto(unsigned int index);
	int			getInclinacionObjeto(unsigned int index);
	float		getBaseMayorObjeto(unsigned int index);
	float		getBaseMenorObjeto(unsigned int index);
	float 		getMasaObjeto(unsigned int index);
	bool 		esObjetoEstatico(unsigned int index);
	std::string getObjectDefinition(unsigned int index);

	//Metodos utilizados por la vista
	std::string getImagenFondo(){ return escenario_->imagenFondo;};
	int 		getAltoPx(){ return escenario_->altoPx;};
	int 		getAnchoPx(){ return escenario_->anchoPx;};

private:

	//atributos
	std::string jsonFile_;
	std::string path_;
	ParserValidator::escenario_t *escenario_;
	ParserValidator::personaje_t *personaje_;
	std::vector<ParserValidator::objeto_t*> objetos_;

	//Metodos
	bool setDefaultValues();
	bool setValuesFromFile();
	bool parseEscenario(Json::Value root);
	bool parsePersonaje(Json::Value root);
	bool parseObjetos(Json::Value root);

};

#endif /* JSONPARSER_H_ */
