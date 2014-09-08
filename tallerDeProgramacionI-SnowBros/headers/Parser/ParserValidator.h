#ifndef PARSERVALIDATOR_H_
#define PARSERVALIDATOR_H_

#include "JsonParser_C.h"
#include <json/json.h>

/**
 * Clase encargada de hacer validaciones para el json
 */
class ParserValidator {
public:

	static int valAltoPx( Json::Value altoPx );
	static int valAnchoPx( Json::Value anchoPx );
	static int valAltoUn( Json::Value altoUn );
	static int valAnchoUn( Json::Value anchoUn );
	static std::string valImagenFondo( Json::Value imagenFondo );
	static float valPersonajeCoorX(float XCoor);
	static float valPersonajeCoorY(float YCoor);
};

#endif /* PARSERVALIDATOR_H_ */
