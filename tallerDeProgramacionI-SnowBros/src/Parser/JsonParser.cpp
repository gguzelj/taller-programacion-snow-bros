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

	escenario = root[ESCENARIO];

	alto_px = escenario.get(ALTO_PX, 2);
	ancho_px = escenario.get(ANCHO_PX, 2);
	alto_un = escenario.get(ALTO_UN, 2);
	ancho_un = escenario.get(ANCHO_UN, 2);
	imagen_fondo = escenario.get(IMAGEN_FONDO, 2);
	personaje = escenario.get(PERSONAJE, 2);

	x = personaje.get(X, 2);
	y = personaje.get(Y, 2);

	objetos = escenario[OBJETOS];

	for (unsigned int i = 0; i < objetos.size(); ++i) {
		tipo = objetos[i].get(TIPO, "rect");
		x = objetos[i].get(X, 5);
		y = objetos[i].get(Y, 90);
		ancho = objetos[i].get(ANCHO, 2);
		alto = objetos[i].get(ALTO, 1);
		color = objetos[i].get(COLOR, "#00FF00");
		rot = objetos[i].get(ROT, 45);
		masa = objetos[i].get(MASA, 3);
		estatico = objetos[i].get(ESTATICO, false);
	}

	std::cout << rot.asInt() << std::endl;
	return;
}
