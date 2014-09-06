#include "../../headers/Parser/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser() {
	return;
}

JsonParser::JsonParser(std::string path) {
	jsonFile_ = FileReader::read(path);
}

/**
 * Destructor
 */
JsonParser::~JsonParser() {
}

/**
 * Realizamos el parseo del archivo
 */
void JsonParser::parse() {

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
	//POdriamos definir un archivo con los valores default
	//jsonFile_ = FileReader::read(DEFAULT_SCENARIO);
	this->setValuesFromFile();
}

/**
 * En los casos donde no se haya asignado un algun archivo json, cargamos los valores
 * del mismo
 */
void JsonParser::setValuesFromFile() {

	bool parsedSuccess;
	b2BodyDef obj;

	parsedSuccess = reader_.parse(jsonFile_, root_, false);

	if (not parsedSuccess) {
		//TODO Agregar error
		return;
	}

	parseEscenario(root_[ESCENARIO]);

	/*
	parsePersonaje(escenario_[PERSONAJE]);


	objetosEscenario_ = escenario_[OBJETOS];

	for (unsigned int i = 0; i < objetos.size(); ++i)
		parseObjeto( objetosEscenario_[index] );
	*/

	return;
}

std::list<b2BodyDef> JsonParser::getObjetos(){
	return objetos_;
}

void JsonParser::parseEscenario(Json::Value escenario){
	alto_px_ = ParserValidator::valAltoPx(escenario.get(ALTO_PX, ALTO_PX_DEF));
	ancho_px_ = ParserValidator::valAnchoPx(escenario.get(ANCHO_PX, ANCHO_PX_DEF));
	alto_un_ = ParserValidator::valAltoUn(escenario.get(ALTO_UN, ALTO_UN_DEF));
	ancho_un_ = ParserValidator::valAnchoUn(escenario.get(ANCHO_UN, ANCHO_UN_DEF));
	imagen_fondo_ = ParserValidator::ImagenFondo(escenario.get(IMAGEN_FONDO, IMAGEN_FONDO_DEF));
}

int JsonParser::getAltoPx(){
	return alto_px_.asInt();
}
int JsonParser::getAnchoPx(){
	return ancho_px_.asInt();
}
int JsonParser::getAltoUn(){
	return alto_un_.asInt();
}
int JsonParser::getAnchoUn(){
	return ancho_un_.asInt();
}

std::string JsonParser::getImagenFondo(){
	return imagen_fondo_.asString();
}
