#include "../../headers/Parser/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser() {
	escenario_ = nullptr;
	return;
}

JsonParser::JsonParser(std::string path) {
	escenario_ = nullptr;
	jsonFile_ = FileReader::read(path);
	this->parse();
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
	//TODO Definir un juego default y guardar ese archivo
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

	parsedSuccess = reader.parse(jsonFile_, root, false);

	if (not parsedSuccess) {
		//TODO Agregar error
		this->setDefaultValues();
		return;
	}

	escenario = root[ESCENARIO];

	parseEscenario(escenario);
	parsePersonaje(escenario[PERSONAJE]);
	parseObjetos(escenario[OBJETOS]);

	return;
}

/**
 * Parseamos la definicion del escenario
 */
void JsonParser::parseEscenario(Json::Value esc) {
	escenario_ = ParserValidator::validarEscenario(esc);
	return;
}

/**
 * Parseamos la definicion del personaje
 */
void JsonParser::parsePersonaje(Json::Value personaje) {
	personaje_ = ParserValidator::validarPersonaje(personaje, escenario_);
	return;
}


/**
 * Parseamos las definiciones de los objetos
 */
void JsonParser::parseObjetos(Json::Value objetos) {

	ParserValidator::objeto_t *obj;

	//Alguno de los objetos puede estar mal definido, y no se deberia tener en cuenta
	for(unsigned int index = 0; index < objetos.size(); index++){

		obj = ParserValidator::validarObjeto(objetos[index], escenario_);

		if(obj != nullptr)
			objetos_.push_back(obj);
	}
}

unsigned int JsonParser::getCantidadObjetos(){
	return objetos_.size();
}

std::string JsonParser::getTipoObjeto(unsigned int index){
	return objetos_[index]->tipo;
}

float JsonParser::getCoorXObjeto(unsigned int index){
	return objetos_[index]->x;
}

float JsonParser::getEscalaObjeto(unsigned int index){
	return objetos_[index]->escala;
}

float JsonParser::getCoorYObjeto(unsigned int index){
	return objetos_[index]->y;
}

float JsonParser::getAnchoObjeto(unsigned int index){
	return objetos_[index]->ancho;
}

float JsonParser::getAltoObjeto(unsigned int index){
	return objetos_[index]->alto;
}

std::string JsonParser::getColorObjeto(unsigned int index){
	return objetos_[index]->color;
}

int	JsonParser::getRotObjeto(unsigned int index){
	return objetos_[index]->rot;
}

float JsonParser::getMasaObjeto(unsigned int index){
	return objetos_[index]->masa;
}

bool JsonParser::esObjetoEstatico(unsigned int index){
	return objetos_[index]->estatico;
}
