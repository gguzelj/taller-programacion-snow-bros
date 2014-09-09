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
 * Parseamos la definicion del escenario
 */
void JsonParser::parseEscenario(Json::Value escenario) {
	escenario_.altoPx = ParserValidator::valAltoPx(escenario.get(ALTO_PX, ALTO_PX_DEF));
	escenario_.anchoPx = ParserValidator::valAnchoPx(escenario.get(ANCHO_PX, ANCHO_PX_DEF));
	escenario_.altoUn = ParserValidator::valAltoUn(escenario.get(ALTO_UN, ALTO_UN_DEF));
	escenario_.anchoUn = ParserValidator::valAnchoUn(escenario.get(ANCHO_UN, ANCHO_UN_DEF));
	escenario_.imagenFondo = ParserValidator::valImagenFondo(escenario.get(IMAGEN_FONDO, IMAGEN_FONDO_DEF));
}

/**
 * Parseamos la definicion del personaje
 */
void JsonParser::parsePersonaje(Json::Value personaje) {
	personaje_.x = ParserValidator::valPersonajeCoorX((personaje.get(X_COOR,X_COOR_DEF)).asFloat());
	personaje_.y = ParserValidator::valPersonajeCoorY((personaje.get(Y_COOR,Y_COOR_DEF)).asFloat());
}


/**
 * Parseamos las definiciones de los objetos
 */
void JsonParser::parseObjetos(Json::Value objetos) {

	objeto_t *obj;

	//Alguno de los objetos puede estar mal definido, y no se deberia tener en cuenta
	for(unsigned int index = 0; index < objetos.size(); index++){

		obj = parseObjeto( objetos[index] );

		if(obj != nullptr)
			objetos_.push_back(obj);
	}
}

JsonParser::objeto_t* JsonParser::parseObjeto(Json::Value objeto){

	JsonParser::objeto_t *ob = new JsonParser::objeto_t();

	ob->tipo = objeto[TIPO].asString();
	ob->x = objeto[X_COOR].asFloat();
	ob->y = objeto[Y_COOR].asFloat();
	ob->escala = objeto[ESCALA].asFloat();
	ob->ancho = objeto[ANCHO].asFloat();
	ob->alto = objeto[ALTO].asFloat();
	ob->color = objeto[COLOR].asString();
	ob->rot = objeto[ROT].asInt();
	ob->masa = objeto[MASA].asFloat();
	objeto[ESTATICO].asString() == "true" ? ob->estatico = true : ob->estatico = false;

	std::cout << "Estatico? " << ob->estatico << std::endl;

	return ob;
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
