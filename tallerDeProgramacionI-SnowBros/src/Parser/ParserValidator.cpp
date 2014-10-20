#include "../../headers/Parser/ParserValidator.h"

/**
 * Realizamos todas las validaciones del escenario. En caso de
 * encontrar algun error en algun atributo, se lo modifica con un
 * valor valido (de ser posible)
 */
bool ParserValidator::validarEscenario(escenario_t* &escenario,
		Json::Value esc) {

	escenario = new ParserValidator::escenario_t();

	//Los siguientes son campos no obligatorios. En caso de que exista algun error
	//sera reemplazado ese valor por uno default, y se guardara registro en el log
	escenario->maximoJugadores = valMaximoJugadores(esc);
	escenario->gravedad = valGravedad(esc);
	escenario->altoPx = valAltoPx(esc);
	escenario->anchoPx = valAnchoPx(esc);
	escenario->altoUn = valAltoUn(esc);
	escenario->anchoUn = valAnchoUn(esc);
	escenario->imagenFondo = valImagenFondo(esc);

	return false;
}

/**
 * Realizamos todas las validaciones del objeto del escenario.
 * En caso de encontrar algun error en algun atributo, se lo
 * modifica con un valor valido (de ser posible). En otro caso
 * se devuelve un false
 */
bool ParserValidator::validarObjeto(objeto_t* &objeto, Json::Value obj,
		escenario_t *esc) {

	objeto = new ParserValidator::objeto_t();

	//Los siguientes campos son obligatorios para el objeto. En caso de que alguno
	//de estos valores sea incorrecto, se setea un nuevo juego por default
	if (valTipoObjeto(obj, objeto->tipo))
		return true;
	if (valCoorXObjeto(obj, objeto->x, esc))
		return true;
	if (valCoorYObjeto(obj, objeto->y, esc))
		return true;
	if (valEscalaObjeto(obj, objeto->escala))
		return true;
	if (valLadosObjeto(obj, objeto->lados))
		return true;
	if (valAnchoObjeto(obj, objeto->ancho))
		return true;
	if (valAltoObjeto(obj, objeto->alto))
		return true;
	if (valBaseObjeto(obj, objeto->base))
		return true;
	if (valEstaticoObjeto(obj, objeto->estatico))
		return true;

	//Los siguientes son atributos no obligatorios en los objetos. En caso de que alguno
	//este mal definido, se setea uno default
	objeto->rot = valRotObjeto(obj);
	objeto->masa = valMasaObjeto(obj);

	return false;
}

/**
 * Validaciones de la cantidad de jugadores
 */
int ParserValidator::valMaximoJugadores(Json::Value esc) {

	int maxiJug;

	if (!esc.isMember(MAXIMO_JUGADORES)) {
		Log::instance()->append(PARSER_MSG_ESC_MXJG, Log::WARNING);
		return MAXIMO_JUGADORES_DEF;
	}

	if (!esc[MAXIMO_JUGADORES].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_MXJG_NO_NUMBER, Log::WARNING);
		return MAXIMO_JUGADORES_DEF;
	}

	maxiJug = esc[MAXIMO_JUGADORES].asInt();

	if (maxiJug > MAXIMO_JUGADORES_MAX || maxiJug < MAXIMO_JUGADORES_MIN) {
		Log::instance()->append(PARSER_MSG_ESC_MXJG_FUERA_RANGO,
				Log::WARNING);
		return MAXIMO_JUGADORES_DEF;
	}

	return maxiJug;

}

/**
 * Validaciones de la gravedad
 */
double ParserValidator::valGravedad(Json::Value esc) {

	if (!esc.isMember(GRAVEDAD)) {
		Log::instance()->append(PARSER_MSG_ESC_GRAVEDAD, Log::WARNING);
		return GRAVEDAD_DEF;
	}

	if (!esc[GRAVEDAD].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_GRAVEDAD_NO_NUMBER,
				Log::WARNING);
		return GRAVEDAD_DEF;
	}

	if (esc[GRAVEDAD] >= 0) {
		Log::instance()->append(PARSER_MSG_ESC_GRAVEDAD_POSITIVA, Log::WARNING);
		return GRAVEDAD_DEF;
	}

	return esc[GRAVEDAD].asDouble();
}

/**
 * Validaciones del alto en pixeles
 */
int ParserValidator::valAltoPx(Json::Value esc) {

	int altoPx;

	if (!esc.isMember(ALTO_PX)) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_PX, Log::WARNING);
		return ALTO_PX_DEF;
	}

	if (!esc[ALTO_PX].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_PX_NO_NUMBER, Log::WARNING);
		return ALTO_PX_DEF;
	}

	altoPx = esc[ALTO_PX].asInt();

	if (altoPx > ALTO_PX_MAX || altoPx < ALTO_PX_MIN) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_PX_FUERA_RANGO,
				Log::WARNING);
		return ALTO_PX_DEF;
	}

	return altoPx;
}

/**
 * Validaciones en el ancho en pixeles
 */
int ParserValidator::valAnchoPx(Json::Value esc) {

	int anchoPx;

	if (!esc.isMember(ANCHO_PX)) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_PX, Log::WARNING);
		return ANCHO_PX_DEF;
	}

	if (!esc[ANCHO_PX].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_PX_NO_NUMBER,
				Log::WARNING);
		return ANCHO_PX_DEF;
	}

	anchoPx = esc[ANCHO_PX].asInt();

	if (anchoPx > ANCHO_PX_MAX || anchoPx < ANCHO_PX_MIN) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_PX_FUERA_RANGO,
				Log::WARNING);
		return ANCHO_PX_DEF;
	}

	return anchoPx;
}

/**
 * Validaciones en el Alto en unidades
 */
double ParserValidator::valAltoUn(Json::Value esc) {

	double altoUn;

	if (!esc.isMember(ALTO_UN)) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_UN, Log::WARNING);
		return ALTO_UN_DEF;
	}

	if (!esc[ALTO_UN].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_UN_NO_NUMBER, Log::WARNING);
		return ALTO_UN_DEF;
	}

	altoUn = esc[ALTO_UN].asDouble();

	if (altoUn > ALTO_UN_MAX || altoUn < ALTO_UN_MIN) {
		Log::instance()->append(PARSER_MSG_ESC_ALTO_UN_FUERA_RANGO,
				Log::WARNING);
		return ALTO_UN_DEF;
	}

	return altoUn;
}

/**
 * Validaciones en el ancho en unidades
 */
double ParserValidator::valAnchoUn(Json::Value esc) {

	double anchoUn;

	if (!esc.isMember(ANCHO_UN)) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_UN, Log::WARNING);
		return ANCHO_UN_DEF;
	}

	if (!esc[ANCHO_UN].isNumeric()) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_UN_NO_NUMBER,
				Log::WARNING);
		return ANCHO_UN_DEF;
	}

	anchoUn = esc[ANCHO_UN].asDouble();

	if (anchoUn > ANCHO_UN_MAX || anchoUn < ANCHO_UN_MIN) {
		Log::instance()->append(PARSER_MSG_ESC_ANCHO_UN_FUERA_RANGO,
				Log::WARNING);
		return ANCHO_UN_DEF;
	}

	return anchoUn;
}

/**
 * Validamos la imagen de fondo del escenario
 */
std::string ParserValidator::valImagenFondo(Json::Value esc) {

	std::string imagenFondo;

	if (!esc.isMember(IMAGEN_FONDO)) {
		Log::instance()->append(PARSER_MSG_ESC_IMAGEN_FONDO, Log::WARNING);
		return IMAGEN_FONDO_DEF;
	}

	if (!esc[IMAGEN_FONDO].isString()) {
		Log::instance()->append(PARSER_MSG_ESC_IMAGEN_FONDO_NO_STRING,
				Log::WARNING);
		return IMAGEN_FONDO_DEF;
	}

	imagenFondo = esc[IMAGEN_FONDO].asString();

	std::ifstream file(imagenFondo);
	if (!file.good()) {
		Log::instance()->append(PARSER_MSG_ESC_IMAGEN_FONDO_NO_EXISTE,
				Log::WARNING);
		return IMAGEN_FONDO_DEF;
	}

	return imagenFondo;
}
/**
 * Validaciones en el tipo de objeto
 */
bool ParserValidator::valTipoObjeto(Json::Value obj, std::string &tipo) {

	if (!obj.isMember(TIPO)) {
		Log::instance()->append(PARSER_MSG_OBJ_TIPO + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[TIPO].isString()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_TIPO_NO_STRING + obj.toStyledString(), Log::WARNING);
		return true;
	}

	tipo = obj[TIPO].asString();

	if (tipo != RECTANGULO && tipo != CIRCULO && tipo != PARALELOGRAMO
			&& tipo != POLIGONO && tipo != TRAPECIO) {
		Log::instance()->append(
		PARSER_MSG_OBJ_TIPO_DESCONOCIDO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos la coordenada X del objeto
 */
bool ParserValidator::valCoorXObjeto(Json::Value obj, double &x,
		escenario_t *esc) {

	if (!obj.isMember(X_COOR)) {
		Log::instance()->append(PARSER_MSG_OBJ_COOR_X + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[X_COOR].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_COOR_X_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	x = obj[X_COOR].asDouble();
	double margen = esc->anchoUn / 2;

	//Validamos que la coordenada X exista dentro del escenario del juego
	if (x > margen || x < -margen) {
		Log::instance()->append(
		PARSER_MSG_OBJ_COOR_X_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos la coordenada Y del objeto
 */
bool ParserValidator::valCoorYObjeto(Json::Value obj, double &y,
		escenario_t *esc) {

	if (!obj.isMember(Y_COOR)) {
		Log::instance()->append(PARSER_MSG_OBJ_COOR_Y + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[Y_COOR].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_COOR_Y_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	y = obj[Y_COOR].asDouble();
	double margen = esc->altoUn / 2;

	//Validamos que la coordenada X exista dentro del escenario del juego
	if (y > margen || y < -margen) {
		Log::instance()->append(
		PARSER_MSG_OBJ_COOR_Y_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos la escala del objeto
 * Para que valide correctamente ya se tendria que haber validado el tipo de objeto
 */
bool ParserValidator::valEscalaObjeto(Json::Value obj, double &escala) {

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Los rectangulos, paralelogramos y trapecios no tienen escala
	if (tipoObjeto == RECTANGULO || tipoObjeto == PARALELOGRAMO
			|| tipoObjeto == TRAPECIO)
		return false;

	//En cualquier otro caso, es un atributo obligatorio
	if (!obj.isMember(ESCALA)) {
		Log::instance()->append(PARSER_MSG_OBJ_ESCALA + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[ESCALA].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ESCALA_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	escala = obj[ESCALA].asDouble();

	//Validamos si la escala existe entre el rango permitido
	if (escala < ESCALA_MIN || escala > ESCALA_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ESCALA_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos la cantidad de lados del objeto
 */
bool ParserValidator::valLadosObjeto(Json::Value obj, int &lados) {

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Los poligonos solo son los unicos que necesitan definir lados
	if (tipoObjeto != POLIGONO)
		return false;

	if (!obj.isMember(LADOS)) {
		Log::instance()->append(PARSER_MSG_OBJ_LADOS + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[LADOS].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_LADOS_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	lados = obj[LADOS].asInt();

	if (lados < LADOS_MIN || lados > LADOS_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_LADOS_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos el ancho del objeto
 */
bool ParserValidator::valAnchoObjeto(Json::Value obj, double &ancho) {

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Solo los rectangulos y paralelogramos tienen este atributo
	if (tipoObjeto != RECTANGULO && tipoObjeto != PARALELOGRAMO)
		return false;

	if (!obj.isMember(ANCHO)) {
		Log::instance()->append(PARSER_MSG_OBJ_ANCHO + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[ANCHO].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ANCHO_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	ancho = obj[ANCHO].asDouble();

	if (ancho < ANCHO_MIN || ancho > ANCHO_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ANCHO_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos el alto del objeto
 */
bool ParserValidator::valAltoObjeto(Json::Value obj, double &alto) {

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Solo los rectangulos, paralelogramos y trapecios tienen este atributo.
	if (tipoObjeto != RECTANGULO && tipoObjeto != PARALELOGRAMO
			&& tipoObjeto != TRAPECIO)
		return false;

	if (!obj.isMember(ALTO)) {
		Log::instance()->append(PARSER_MSG_OBJ_ALTO + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[ALTO].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ALTO_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	alto = obj[ALTO].asDouble();

	if (alto < ALTO_MIN || alto > ALTO_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ALTO_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;
}

/**
 * Validamos la base mayor y menor del paralelogramo.
 */
bool ParserValidator::valBaseObjeto(Json::Value obj, double &base) {

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Solo los trapecios tienen este atributo
	if (tipoObjeto != TRAPECIO)
		return false;

	if (!obj.isMember(BASE)) {
		Log::instance()->append(PARSER_MSG_OBJ_BASE + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[BASE].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_BASE_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return true;
	}

	base = obj[BASE].asDouble();

	if (base < BASE_MIN || base > BASE_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_BASE_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return true;
	}

	return false;

}

/**
 * Validaciones en flag de estatico del objeto
 */
bool ParserValidator::valEstaticoObjeto(Json::Value obj, bool &estatico) {

	if (!obj.isMember(ESTATICO)) {
		Log::instance()->append(PARSER_MSG_OBJ_ESTATICO + obj.toStyledString(),
				Log::WARNING);
		return true;
	}

	if (!obj[ESTATICO].isBool()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ESTATICO_NO_BOOL + obj.toStyledString(), Log::WARNING);
		return true;
	}

	estatico = obj[ESTATICO].asBool();

	return false;
}

/**
 * Validamos la rotacion del objeto
 */
int ParserValidator::valRotObjeto(Json::Value obj) {

	int rotacion;

	//Podemos leer este atributo porque ya sabemos que es valido
	std::string tipoObjeto = obj[TIPO].asString();

	//Los circulo no tienen rotacion
	if (tipoObjeto == CIRCULO)
		return false;

	if (!obj.isMember(ROT)) {
		Log::instance()->append(PARSER_MSG_OBJ_ROT + obj.toStyledString(),
				Log::WARNING);
		return ROT_DEF;
	}

	if (!obj[ROT].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ROT_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return ROT_DEF;
	}

	rotacion = obj[ROT].asInt();

	//Validaciones de la rotacion del objeto
	if (rotacion < ROT_MIN || rotacion > ROT_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_ROT_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return ROT_DEF;
	}

	return rotacion;
}

/**
 * Validamos la masa del objeto
 */
int ParserValidator::valMasaObjeto(Json::Value obj) {

	int masa;

	if (!obj.isMember(MASA)) {
		Log::instance()->append(PARSER_MSG_OBJ_MASA + obj.toStyledString(),
				Log::WARNING);
		return MASA_DEF;
	}

	if (!obj[MASA].isNumeric()) {
		Log::instance()->append(
		PARSER_MSG_OBJ_MASA_NO_NUMBER + obj.toStyledString(), Log::WARNING);
		return MASA_DEF;
	}

	masa = obj[MASA].asInt();

	//Validaciones de la masa del objeto
	if (masa < MASA_MIN || masa > MASA_MAX) {
		Log::instance()->append(
		PARSER_MSG_OBJ_MASA_FUERA_RANGO + obj.toStyledString(), Log::WARNING);
		return MASA_DEF;
	}

	return masa;
}
