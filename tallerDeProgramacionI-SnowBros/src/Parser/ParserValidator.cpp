#include "../../headers/Parser/ParserValidator.h"

//TODO Agregar errores en caso de que existan
Json::Value ParserValidator::valAltoPx( Json::Value altoPx ){

	if( altoPx.asInt() < 2048 && altoPx.asInt() > 200)
		return altoPx;

	return ALTO_PX_DEF;
}

Json::Value ParserValidator::valAnchoPx( Json::Value anchoPx ){
	return anchoPx;
}

Json::Value ParserValidator::valAltoUn( Json::Value altoUn ){
	return altoUn;
}

Json::Value ParserValidator::valAnchoUn( Json::Value anchoUn ){
	return anchoUn;
}

Json::Value ParserValidator::ImagenFondo( Json::Value imagenFondo ){
	return imagenFondo;
}


