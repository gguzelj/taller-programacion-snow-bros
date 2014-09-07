#include "../../headers/Parser/JsonParser.h"

/**
 * Constructor
 */
JsonParser::JsonParser() {
	return;
}

JsonParser::JsonParser(std::string path) {
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
		return;
	}

	escenario = root[ESCENARIO];

	parseEscenario(escenario);
	parsePersonaje(escenario[PERSONAJE]);
	parseObjetos(escenario[OBJETOS]);

	return;
}

/**
 * Devolvemos la definicion de los objetos de juego
 */
std::list<b2BodyDef> JsonParser::getObjetos() {
	return objetos_;
}

/**
 * Parseamos la definicion del escenario
 */
void JsonParser::parseEscenario(Json::Value escenario) {
	alto_px_ = ParserValidator::valAltoPx(escenario.get(ALTO_PX, ALTO_PX_DEF));
	ancho_px_ = ParserValidator::valAnchoPx(escenario.get(ANCHO_PX, ANCHO_PX_DEF));
	alto_un_ = ParserValidator::valAltoUn(escenario.get(ALTO_UN, ALTO_UN_DEF));
	ancho_un_ = ParserValidator::valAnchoUn(escenario.get(ANCHO_UN, ANCHO_UN_DEF));
	imagen_fondo_ = ParserValidator::valImagenFondo(escenario.get(IMAGEN_FONDO, IMAGEN_FONDO_DEF));
}

/**
 * Parseamos la definicion del personaje
 */
void JsonParser::parsePersonaje(Json::Value personaje) {
	personaje_.type = b2_dynamicBody;
	personaje_.angle = 0;
	personaje_.position.Set(ParserValidator::valPersonajeCoorX((personaje.get(X_COOR,X_COOR_DEF)).asFloat()) ,
							ParserValidator::valPersonajeCoorY((personaje.get(Y_COOR,Y_COOR_DEF)).asFloat()));
}


/**
 * Parseamos las definiciones de los objetos
 */
void JsonParser::parseObjetos(Json::Value objetos) {

	for(unsigned int index = 0; index < objetos.size(); index++)
		parseObjeto( objetosEscenario_[index] );
}

/**
 * Parseamos la definicion del objeto
 */
void JsonParser::parseObjeto(Json::Value objeto) {

	b2BodyDef obj;

	obj.type = (objeto[ESTATICO].asBool())?b2_staticBody:b2_dynamicBody;
	obj.angle = 0;

	objetos_.push_back(obj);

}

/**
 * Devolvemos el alto en pixeles
 */
int JsonParser::getAltoPx() {
	return alto_px_.asInt();
}

/**
 * Devolvemos el ancho en pixeles
 */
int JsonParser::getAnchoPx() {
	return ancho_px_.asInt();
}

/**
 * Devolvemos el alto en unidades
 */
int JsonParser::getAltoUn() {
	return alto_un_.asInt();
}

/**
 * Devolvemos el ancho en unidades
 */
int JsonParser::getAnchoUn() {
	return ancho_un_.asInt();
}

/**
 * Devolvemos la definicion del personaje
 */
b2BodyDef JsonParser::getPersonaje(){
	return personaje_;
}

/**
 * Devolvemos la imagen de fondo
 */
std::string JsonParser::getImagenFondo() {
	return imagen_fondo_.asString();
}
