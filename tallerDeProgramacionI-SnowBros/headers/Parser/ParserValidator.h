#ifndef PARSERVALIDATOR_H_
#define PARSERVALIDATOR_H_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <typeinfo>
#include <arpa/inet.h>

#include "JsonParser_C.h"
#include <json/json.h>
#include "../Log/Log.h"

#define FACTOR_CONVERSION_UN_A_PX 25
#define imagePath "resources/snowBackground.png"

/**
 * Clase encargada de hacer validaciones para el json
 */
class ParserValidator {
public:

	class escenario_t {
	public:
		int maximoJugadores;
		double gravedad;
		double altoUn;
		double anchoUn;
	};

	class personaje_t{
	public:
		double x;
		double y;
	};

	class objeto_t {
	public:
		std::string tipo;
		double x;
		double y;
		double escala;
		int lados;
		double ancho;
		double alto;
		int rot;
		double base;
		int masa;
		bool estatico;
		Json::Value objectDefinition;
	};

	static bool validarEscenario(escenario_t* &escenario, Json::Value esc);
	static bool validarObjeto(objeto_t* &objeto, Json::Value obj, escenario_t *esc);

private:

	//Validaciones en el escenario
	static int valMaximoJugadores(Json::Value esc);
	static double valGravedad(Json::Value esc);
	static double valAltoUn(Json::Value esc);
	static double valAnchoUn(Json::Value esc);

	//Validaciones de los objetos
	static bool valTipoObjeto(Json::Value obj, std::string &tipo);
	static bool valCoorXObjeto(Json::Value obj, double &x, escenario_t *esc);
	static bool valCoorYObjeto(Json::Value obj, double &y, escenario_t *esc);
	static bool valEscalaObjeto(Json::Value obj, double &escala);
	static bool valLadosObjeto(Json::Value obj, int &lados);
	static bool valAnchoObjeto(Json::Value obj, double &ancho);
	static bool valAltoObjeto(Json::Value obj, double &alto);
	static bool valBaseObjeto(Json::Value obj, double &base);
	static bool valEstaticoObjeto(Json::Value obj, bool &estatico);
	static int valRotObjeto(Json::Value obj);
	static int valMasaObjeto(Json::Value obj);

};

#endif /* PARSERVALIDATOR_H_ */
