#include "../../headers/Parser/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser(std::string path) {
	escenario_ = nullptr;
	path_ = path;
}

/**
 * Destructor
 */
JsonParser::~JsonParser() {
}

/**
 * Realizamos el parseo del archivo
 */
bool JsonParser::parse() {

	Log::ins()->add(PARSER_MSG_START_PARSING + path_, Log::INFO);

	jsonFile_ = FileReader::read(path_);

	if (jsonFile_.empty()) {
		Log::ins()->add(PARSER_MSG_OPEN_FILE + path_, Log::WARNING);
		return this->setDefaultValues();
	} else {
		return this->setValuesFromFile();
	}
}

/**
 * En los casos donde no se haya seteado ningun archivo json, cargamos juego
 * con los valores por default
 */
bool JsonParser::setDefaultValues() {
	Log::ins()->add(PARSER_MSG_DEFAULT_GAME, Log::INFO);
	path_ = PATH_DEF;
	jsonFile_ = FileReader::read(path_);
	return this->setValuesFromFile();
}

/**
 * En los casos donde no se haya asignado un algun archivo json, cargamos los valores
 * del mismo
 */
bool JsonParser::setValuesFromFile() {

	bool parsedSuccess;
	Json::Reader *reader = new Json::Reader(Json::Features::strictMode());
	Json::Value root;

	parsedSuccess = reader->parse(jsonFile_, root, false);

	if (not parsedSuccess) {
		Log::ins()->add(PARSER_MSG_INVALID_JSON + reader->getFormatedErrorMessages(),Log::WARNING);
		if (path_ != PATH_DEF)
			return this->setDefaultValues();

		Log::ins()->add(PARSER_MSG_INVALID_DEF_JSON,Log::ERROR);
		return true;
	}

	//Parseamos el escenario
	if(parseEscenario(root)){
		delete escenario_;
		if(path_ != PATH_DEF) return this->setDefaultValues();
		Log::ins()->add(PARSER_MSG_INVALID_DEF_JSON,Log::ERROR);
		return true;
	}

	//Parseamos los objetos
	if(parseObjetos(root)){
		delete escenario_;
		for(unsigned int i = 0; i < objetos_.size(); i++) delete objetos_[i];
		if(path_ != PATH_DEF) return this->setDefaultValues();
		Log::ins()->add(PARSER_MSG_INVALID_DEF_JSON,Log::ERROR);
		return true;
	}

	Log::ins()->add(PARSER_MSG_JSON_OK,Log::INFO);
	return false;
}

/**
 * Parseamos la definicion del escenario
 */
bool JsonParser::parseEscenario(Json::Value root) {

	Log::ins()->add(PARSER_MSG_COMIENZO_ESCENARIO, Log::INFO);

	if (!root.isMember(ESCENARIO)) {
		Log::ins()->add(PARSER_MSG_NO_ESCENARIO, Log::WARNING);
		return true;
	}

	Json::Value escenario = root[ESCENARIO];

	if(!escenario.isObject()){
		Log::ins()->add(PARSER_MSG_TIPO_ESCENARIO, Log::WARNING);
		return true;
	}

	return ParserValidator::validarEscenario(escenario_, escenario);
}


/**
 * Parseamos las definiciones de los objetos
 */
bool JsonParser::parseObjetos(Json::Value root) {

	Log::ins()->add(PARSER_MSG_COMIENZO_OBJETOS, Log::INFO);

	ParserValidator::objeto_t *obj;
	Json::Value objetos;
	Json::Value escenario = root[ESCENARIO];

	if (!escenario.isMember(OBJETOS)) {
		Log::ins()->add(PARSER_MSG_NO_OBJETOS, Log::WARNING);
		return true;
	}

	objetos = escenario[OBJETOS];

	if(!objetos.isArray()){
		Log::ins()->add(PARSER_MSG_TIPO_OBJETOS, Log::WARNING);
		return true;
	}

	//Alguno de los objetos puede estar mal definido, y no se deberia tener en cuenta
	for (unsigned int index = 0; index < objetos.size(); index++) {
		if(!ParserValidator::validarObjeto(obj, objetos[index], escenario_)){
			obj->objectDefinition = objetos[index];
			objetos_.push_back(obj);
		}
	}

	if(objetos_.empty()){
		Log::ins()->add(PARSER_MSG_SIN_OBJETOS, Log::WARNING);
		return true;
	}

	return false;
}

unsigned int JsonParser::getCantidadObjetos() {
	return objetos_.size();
}

std::string JsonParser::getTipoObjeto(unsigned int index) {
	return objetos_[index]->tipo;
}

float JsonParser::getCoorXObjeto(unsigned int index) {
	return objetos_[index]->x;
}

float JsonParser::getEscalaObjeto(unsigned int index) {
	return objetos_[index]->escala;
}

int JsonParser::getLadosObjeto(unsigned int index){
	return objetos_[index]->lados;
}

float JsonParser::getCoorYObjeto(unsigned int index) {
	return objetos_[index]->y;
}

float JsonParser::getAnchoObjeto(unsigned int index) {
	return objetos_[index]->ancho;
}

float JsonParser::getAltoObjeto(unsigned int index) {
	return objetos_[index]->alto;
}

int JsonParser::getRotObjeto(unsigned int index) {
	return objetos_[index]->rot;
}

float JsonParser::getBaseObjeto(unsigned int index){
	return objetos_[index]->base;
}
float JsonParser::getMasaObjeto(unsigned int index) {
	return objetos_[index]->masa;
}

bool JsonParser::esObjetoEstatico(unsigned int index) {
	return objetos_[index]->estatico;
}

std::string JsonParser::getObjectDefinition(unsigned int index){
	return objetos_[index]->objectDefinition.toStyledString();
}
