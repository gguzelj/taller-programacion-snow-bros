#include "../../headers/Parser/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser(std::string path) {
	escenario_ = nullptr;
	personaje_ = nullptr;
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

	jsonFile_ = FileReader::read(path_);

	if (jsonFile_.empty()) {
		Log::instance()->append(PARSER_ERROR_OPEN_FILE, Log::WARNING);
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
	Log::instance()->append(PARSER_INFO_DEFAULT_GAME, Log::INFO);
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
	Json::Reader reader;
	Json::Value root;
	Json::Value escenario;

	parsedSuccess = reader.parse(jsonFile_, root, false);

	if (not parsedSuccess) {
		Log::instance()->append(PARSER_ERROR_INVALID_JSON + reader.getFormatedErrorMessages(),Log::ERROR);
		if (path_ != PATH_DEF)
			return this->setDefaultValues();
		else
			return true;
	}

	if (parseEscenario(root))return true;//this->setDefaultValues();
	if (parsePersonaje(root))return true;//this->setDefaultValues();
	if (parseObjetos(root))return true;//this->setDefaultValues();

	return false;
}

/**
 * Parseamos la definicion del escenario
 */
bool JsonParser::parseEscenario(Json::Value root) {

	if (!root.isMember(ESCENARIO)) {
		Log::instance()->append(PARSER_ERROR_NO_ESCENARIO, Log::ERROR);
		return true;
	}

	Json::Value escenario = root[ESCENARIO];

	if(!escenario.isObject()){
		Log::instance()->append(PARSER_ERROR_TIPO_ESCENARIO, Log::ERROR);
		return true;
	}

	return ParserValidator::validarEscenario(escenario_, escenario);
}

/**
 * Parseamos la definicion del personaje
 */
bool JsonParser::parsePersonaje(Json::Value root) {

	Json::Value escenario = root[ESCENARIO];

	if (!escenario.isMember(PERSONAJE)) {
		Log::instance()->append(PARSER_ERROR_NO_PERSONAJE, Log::ERROR);
		return true;
	}

	Json::Value personaje = escenario[PERSONAJE];

	if (!personaje.isObject()) {
		Log::instance()->append(PARSER_ERROR_TIPO_PERSONAJE, Log::ERROR);
		return true;
	}

	return ParserValidator::validarPersonaje(personaje_,personaje,escenario_);
}

/**
 * Parseamos las definiciones de los objetos
 */
bool JsonParser::parseObjetos(Json::Value root) {

	ParserValidator::objeto_t *obj;
	Json::Value objetos;
	Json::Value escenario = root[ESCENARIO];

	if (!escenario.isMember(OBJETOS)) {
		Log::instance()->append(PARSER_ERROR_NO_OBJETOS, Log::ERROR);
		return true;
	}

	objetos = escenario[OBJETOS];

	if(!objetos.isArray()){
		Log::instance()->append(PARSER_ERROR_TIPO_OBJETOS, Log::ERROR);
		return true;
	}

	//Alguno de los objetos puede estar mal definido, y no se deberia tener en cuenta
	for (unsigned int index = 0; index < objetos.size(); index++) {
		if(!ParserValidator::validarObjeto(obj, objetos[index], escenario_))
			objetos_.push_back(obj);
	}


	if(objetos_.empty()){
		Log::instance()->append(PARSER_ERROR_SIN_OBJETOS, Log::ERROR);
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

std::string JsonParser::getColorObjeto(unsigned int index) {
	return objetos_[index]->color;
}

int JsonParser::getRotObjeto(unsigned int index) {
	return objetos_[index]->rot;
}

float JsonParser::getMasaObjeto(unsigned int index) {
	return objetos_[index]->masa;
}

bool JsonParser::esObjetoEstatico(unsigned int index) {
	return objetos_[index]->estatico;
}
