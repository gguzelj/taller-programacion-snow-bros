#ifndef PARSERVALIDATOR_H_
#define PARSERVALIDATOR_H_

#include "JsonParser_C.h"
#include <json/json.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>

/**
 * Clase encargada de hacer validaciones para el json
 */
class ParserValidator {
public:

	class escenario_t {
	public:
		int altoPx;
		int anchoPx;
		float altoUn;
		float anchoUn;
		std::string imagenFondo;
	};

	class personaje_t{
	public:
		float x;
		float y;
	};

	class objeto_t {
	public:
		std::string tipo;
		float x;
		float y;
		float escala;
		int ancho;
		int alto;
		std::string color;
		int rot;
		int masa;
		bool estatico;
	};

	static escenario_t* validarEscenario(Json::Value esc);
	static personaje_t* validarPersonaje(Json::Value per, escenario_t *esc);
	static objeto_t* validarObjeto(Json::Value obj, escenario_t *esc);

private:

	//Validaciones en el escenario
	static int valAltoPx(Json::Value esc);
	static int valAnchoPx(Json::Value esc);
	static int valAltoUn(Json::Value esc);
	static int valAnchoUn(Json::Value esc);
	static std::string valImagenFondo(Json::Value esc);

	//Validaciones en el personaje
	static float valPersonajeCoorX(Json::Value per, escenario_t *esc);
	static float valPersonajeCoorY(Json::Value per, escenario_t *esc);

	//Validaciones de los objetos
	static std::string valTipoObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static float valCoorXObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static float valCoorYObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static float valEscalaObjeto(Json::Value obj, escenario_t *esc, std::string tipoObjeto, bool &error);
	static int valAnchoObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static int valAltoObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static std::string valColorObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static int valRotObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static int valMasaObjeto(Json::Value obj, escenario_t *esc, bool &error);
	static bool valEstaticoObjeto(Json::Value obj, escenario_t *esc, bool &error);

};

#endif /* PARSERVALIDATOR_H_ */
