#include "../../headers/Parser/ParserValidator.h"

/**
 * Realizamos todas las validaciones del escenario. En caso de
 * encontrar algun error en algun atributo, se lo modifica con un
 * valor valido (de ser posible)
 */
ParserValidator::escenario_t* ParserValidator::validarEscenario(Json::Value esc){

	ParserValidator::escenario_t *escenario = new ParserValidator::escenario_t();

	escenario->altoPx = valAltoPx(esc);
	escenario->anchoPx = valAltoPx(esc);
	escenario->altoUn = valAltoUn(esc);
	escenario->anchoUn = valAnchoUn(esc);
	escenario->imagenFondo = valImagenFondo(esc);

	return escenario;
}

/**
 * Realizamos todas las validaciones del personaje. En caso de
 * encontrar algun error en algun atributo, se lo modifica con un
 * valor valido (de ser posible)
 */
ParserValidator::personaje_t* ParserValidator::validarPersonaje(Json::Value per, escenario_t *esc){

	ParserValidator::personaje_t *personaje = new ParserValidator::personaje_t();

	personaje->x = valPersonajeCoorX(per, esc);
	personaje->y = valPersonajeCoorY(per, esc);

	return personaje;
}

/**
 * Realizamos todas las validaciones del objeto del escenario.
 * En caso de encontrar algun error en algun atributo, se lo
 * modifica con un valor valido (de ser posible). En otro caso
 * se devuelve un false
 */
ParserValidator::objeto_t* ParserValidator::validarObjeto(Json::Value obj, escenario_t *esc){

	ParserValidator::objeto_t *objeto = new ParserValidator::objeto_t();
	bool existeError = false;

	objeto->tipo = valTipoObjeto(obj, esc, existeError);
	objeto->x = valCoorXObjeto(obj, esc, existeError);
	objeto->y = valCoorYObjeto(obj, esc, existeError);
	objeto->escala = valEscalaObjeto(obj, esc, objeto->tipo, existeError);
	objeto->ancho = valAnchoObjeto(obj, esc, existeError);
	objeto->alto = valAltoObjeto(obj, esc, existeError);
	objeto->color = valColorObjeto(obj, esc, existeError);
	objeto->rot = valRotObjeto(obj, esc, existeError);
	objeto->masa = valMasaObjeto(obj, esc, existeError);
	objeto->estatico = valEstaticoObjeto(obj, esc, existeError);

	if(existeError){
		delete objeto;
		return nullptr;
	}

	return objeto;
}


/**
 * Validaciones del alto en pixeles
 */
int ParserValidator::valAltoPx( Json::Value esc ){

	int altoPx;

	if(! esc[ALTO_PX].isInt()){
		//TODO agregar mensaje en el log
		altoPx = ALTO_PX_DEF;
	} else {
		altoPx = esc.get(ALTO_PX, ALTO_PX_DEF).asInt();
	}

	if( altoPx > ALTO_PX_MAX || altoPx < ALTO_PX_MIN ){
		altoPx = ALTO_PX_DEF;
		//TODO Agregar mensaje en el log
	}

	return altoPx;
}

/**
 * Validaciones en el ancho en pixeles
 */
int ParserValidator::valAnchoPx( Json::Value esc ){

	int anchoPx;

	if(! esc[ANCHO_PX].isInt()){
		//TODO agregar mensaje en el log
		anchoPx = ANCHO_PX_DEF;
	} else {
		anchoPx = esc.get(ANCHO_PX, ANCHO_PX_DEF).asInt();
	}

	if( anchoPx > ANCHO_PX_MAX || anchoPx < ANCHO_PX_MIN ){
		anchoPx = ANCHO_PX_DEF;
		//TODO Agregar mensaje en el log
	}

	return anchoPx;
}

/**
 * Validaciones en el Alto en unidades
 */
int ParserValidator::valAltoUn( Json::Value esc ){

	int altoUn;

	if(! esc[ALTO_UN].isInt()){
		//TODO agregar mensaje en el log
		altoUn = ALTO_UN_DEF;
	} else {
		altoUn = esc.get(ALTO_UN, ALTO_UN_DEF).asInt();
	}

	if( altoUn > ALTO_UN_MAX || altoUn < ALTO_UN_MIN ){
		altoUn = ALTO_UN_DEF;
		//TODO Agregar mensaje en el log
	}

	return altoUn;
}

/**
 * Validaciones en el ancho en unidades
 */
int ParserValidator::valAnchoUn( Json::Value esc ){

	int anchoUn;

	if(! esc[ANCHO_UN].isInt()){
		//TODO agregar mensaje en el log
		anchoUn = ANCHO_UN_DEF;
	} else {
		anchoUn = esc.get(ANCHO_UN, ANCHO_UN_DEF).asInt();
	}

	if( anchoUn > ANCHO_UN_MAX || anchoUn < ANCHO_UN_MIN ){
		anchoUn = ANCHO_UN_DEF;
		//TODO Agregar mensaje en el log
	}

	return anchoUn;
}

/**
 * Validamos la imagen de fondo del escenario
 */
std::string ParserValidator::valImagenFondo( Json::Value esc ){

	std::string imagenFondo;

	if(! esc[IMAGEN_FONDO].isString()){
		//TODO agregar mensaje en el log
		imagenFondo = IMAGEN_FONDO_DEF;
	} else {
		imagenFondo = esc.get(IMAGEN_FONDO, IMAGEN_FONDO_DEF).asString();
	}

	std::ifstream file(imagenFondo);
	if (!file.good()){
		imagenFondo = IMAGEN_FONDO_DEF;
		//TODO Agregar mensaje en el log
	}

	return imagenFondo;
}

/**
 * Validamos la coordenada X del personaje
 */
float ParserValidator::valPersonajeCoorX(Json::Value per, escenario_t *esc){

	double coorX;

	//Validamos la coordenada X del personaje
	if(! per[X_COOR].isDouble()){
		//TODO agregar mensaje en el log
		coorX = X_COOR_DEF;
	} else {
		coorX = per.get(X_COOR, X_COOR_DEF).asDouble();
	}

	//Validaciones del personaje
	if(	coorX < 0 || coorX > esc->anchoPx ){
		coorX = X_COOR_DEF;
	}

	return (float) coorX;
}

/**
 * Validamos la coordenada Y del personaje
 */
float ParserValidator::valPersonajeCoorY(Json::Value per, escenario_t *esc){

	double coorY;

	//Validamos la coordenada X del personaje
	if(! per[Y_COOR].isDouble()){
		//TODO agregar mensaje en el log
		coorY = Y_COOR_DEF;
	} else {
		coorY = per.get(Y_COOR, Y_COOR_DEF).asDouble();
	}

	//Validaciones del personaje
	if(	coorY < 0 || coorY > esc->anchoPx ){
		coorY = Y_COOR_DEF;
	}

	return (float) coorY;
}

/**
 * Validaciones en el tipo de objeto
 */
std::string ParserValidator::valTipoObjeto(Json::Value obj, escenario_t *esc, bool &error){

	std::string tipoObjeto;

	if(! obj[TIPO].isString()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		tipoObjeto = obj[TIPO].asString();
	}

	if(	tipoObjeto != RECTANGULO && tipoObjeto != CIRCULO &&
		tipoObjeto != PARALELOGRAMO && tipoObjeto != POLIGONO ){
		//TODO Agregar error
		error = true;
	}

	return tipoObjeto;
}

/**
 * Validamos la coordenada X del objeto
 */
float ParserValidator::valCoorXObjeto(Json::Value obj, escenario_t *esc, bool &error){

	double coorX;

	//Validamos la coordenada X del objeto
	if(! obj[X_COOR].isDouble()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		coorX = obj[X_COOR].asDouble();
	}

	//Validaciones de la coordenada
	if(	coorX < 0 || coorX > esc->anchoPx ){
		error = true;
	}

	return (float) coorX;
}

/**
 * Validamos la coordenada Y del objeto
 */
float ParserValidator::valCoorYObjeto(Json::Value obj, escenario_t *esc, bool &error){

	double coorY;

	//Validamos la coordenada Y del objeto
	if(! obj[Y_COOR].isDouble()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		coorY = obj[Y_COOR].asDouble();
	}

	//Validaciones de la coordenada
	//if(	coorY < 0 || coorY > esc->altoPx ){
		//error = true;
	//}

	return (float) coorY;
}

/**
 * Validamos la escala del objeto
 */
float ParserValidator::valEscalaObjeto(Json::Value obj, escenario_t *esc, std::string tipoObjeto, bool &error){

	double escala;

	if(tipoObjeto == RECTANGULO) return 0;

	//Validamos la escala del objeto
	if(! obj[ESCALA].isDouble()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		escala = obj.get(ESCALA, -1).asDouble();
	}

	//Validaciones de la coordenada
	if(	escala < ESCALA_MIN || escala > ESCALA_MAX ){
		error = true;
	}

	return (float) escala;
}

/**
 * Validamos el ancho del objeto
 */
int ParserValidator::valAnchoObjeto(Json::Value obj, escenario_t *esc, bool &error){

	double ancho;

	//Validamos el ancho del objeto
	if(! obj[ANCHO].isInt()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		ancho = obj[ANCHO].asInt();
	}

	//Validaciones del ancho del objeto
	if(	ancho < ANCHO_MIN || ancho > ANCHO_MAX ){
		error = true;
	}

	return ancho;
}

/**
 * Validamos el alto del objeto
 */
int ParserValidator::valAltoObjeto(Json::Value obj, escenario_t *esc, bool &error){

	double alto;

	//Validamos el alto del objeto
	if(! obj[ALTO].isInt()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		alto = obj[ALTO].asInt();
	}

	//Validaciones de la altura del objeto
	if(	alto < ALTO_MIN || alto > ALTO_MAX ){
		error = true;
	}

	return alto;
}

/**
 * Validamos el color del objeto
 */
std::string ParserValidator::valColorObjeto(Json::Value obj, escenario_t *esc, bool &error){

	std::string color;
	int num;
	unsigned int R,G,B;

	//Validamos el color del objeto
	if(obj[COLOR].isString())
		color = obj[COLOR].asString();

	if(color.size() >= 7 ){
		  std::stringstream ss(color.substr(1,7));
		  ss >> std::hex >> num;

		  R = num / 0x10000;
		  G = (num / 0x100) % 0x100;
		  B = num % 0x100;

	} else {
		color = COLOR_DEF;
	}

	if( ( R >= 0 && R <= 255 ) &&
		( G >= 0 && G <= 255 ) &&
		( B >= 0 && B <= 255 ) ) return color;

	return COLOR_DEF;
}

/**
 * Validamos la rotacion del objeto
 */
int ParserValidator::valRotObjeto(Json::Value obj, escenario_t *esc, bool &error){

	int rotacion;

	//Validamos la rotacion del objeto
	if(! obj[ROT].isInt()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		rotacion = obj[ROT].asInt();
	}

	//Validaciones de la rotacion del objeto
	if(	rotacion < ROT_MIN || rotacion > ROT_MAX ){
		rotacion = ROT_DEF;
		//TODO Agregar error
	}

	return rotacion;
}

/**
 * Validamos la masa del objeto
 */
int ParserValidator::valMasaObjeto(Json::Value obj, escenario_t *esc, bool &error){

	int masa;

	//Validamos la masa del objeto
	if(! obj[MASA].isInt()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		masa = obj[MASA].asInt();
	}

	//Validaciones de la masa del objeto
	if(	masa < MASA_MIN || masa > MASA_MAX ){
		masa = MASA_DEF;
		//TODO Agregar error
	}

	return masa;
}

/**
 * Validaciones en flag de estatico del objeto
 */
bool ParserValidator::valEstaticoObjeto(Json::Value obj, escenario_t *esc, bool &error){

	int estatico;

	//Validamos el flag estatico del objeto
	if(! obj[ESTATICO].isBool()){
		//TODO agregar mensaje en el log
		error = true;
	} else {
		estatico = obj[ESTATICO].asBool();
	}

	return estatico;
}
