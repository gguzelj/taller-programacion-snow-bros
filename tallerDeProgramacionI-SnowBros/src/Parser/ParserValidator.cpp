#include "../../headers/Parser/ParserValidator.h"

//TODO Agregar errores en caso de que existan
int ParserValidator::valAltoPx( Json::Value altoPx ){

	if( altoPx.asInt() < 2048 && altoPx.asInt() > 200)
		return altoPx.asInt();

	return ALTO_PX_DEF;
}

int ParserValidator::valAnchoPx( Json::Value anchoPx ){
	return anchoPx.asInt();
}

int ParserValidator::valAltoUn( Json::Value altoUn ){
	return altoUn.asInt();
}

int ParserValidator::valAnchoUn( Json::Value anchoUn ){
	return anchoUn.asInt();
}

std::string ParserValidator::valImagenFondo( Json::Value imagenFondo ){
	return imagenFondo.asString();
}

float ParserValidator::valPersonajeCoorX(float XCoor){
	return XCoor;
}
float ParserValidator::valPersonajeCoorY(float YCoor){
	return YCoor;
}


