#ifndef PARSERVALIDATOR_H_
#define PARSERVALIDATOR_H_

#include "JsonParser_C.h"
#include <json/json.h>

/**
 * Clase encargada de hacer validaciones para el json
 */
class ParserValidator {
public:

	static Json::Value valAltoPx( Json::Value altoPx );
	static Json::Value valAnchoPx( Json::Value anchoPx );
	static Json::Value valAltoUn( Json::Value altoUn );
	static Json::Value valAnchoUn( Json::Value anchoUn );
	static Json::Value valImagenFondo( Json::Value imagenFondo );
	static float valPersonajeCoorX(float XCoor);
	static float valPersonajeCoorY(float YCoor);
};

#endif /* PARSERVALIDATOR_H_ */