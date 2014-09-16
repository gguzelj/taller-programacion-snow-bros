#ifndef PARSERVALIDATOR_H_
#define PARSERVALIDATOR_H_

#include "JsonParser_C.h"
#include <json/json.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "../Log/Log.h"

/**
 * Clase encargada de hacer validaciones para el json
 */
class ParserValidator {
public:

	class escenario_t {
	public:
		int altoPx;
		int anchoPx;
		double altoUn;
		double anchoUn;
		std::string imagenFondo;
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
		std::string color;
		int rot;
		int inclinacion;
		double base_mayor;
		double base_menor;
		int masa;
		bool estatico;
		Json::Value objectDefinition;
	};

	static bool validarEscenario(escenario_t* &escenario, Json::Value esc);
	static bool validarPersonaje(personaje_t* &personaje, Json::Value per, escenario_t *esc);
	static bool validarObjeto(objeto_t* &objeto, Json::Value obj, escenario_t *esc);

private:

	//Validaciones en el escenario
	static int valAltoPx(Json::Value esc);
	static int valAnchoPx(Json::Value esc);
	static double valAltoUn(Json::Value esc);
	static double valAnchoUn(Json::Value esc);
	static std::string valImagenFondo(Json::Value esc);

	//Validaciones en el personaje
	static bool valPersonajeCoorX(Json::Value per, double &x, escenario_t *esc);
	static bool valPersonajeCoorY(Json::Value per, double &y, escenario_t *esc);

	//Validaciones de los objetos
	static bool valTipoObjeto(Json::Value obj, std::string &tipo);
	static bool valCoorXObjeto(Json::Value obj, double &x, escenario_t *esc);
	static bool valCoorYObjeto(Json::Value obj, double &y, escenario_t *esc);
	static bool valEscalaObjeto(Json::Value obj, double &escala);
	static bool valLadosObjeto(Json::Value obj, int &lados);
	static bool valAnchoObjeto(Json::Value obj, double &ancho);
	static bool valAltoObjeto(Json::Value obj, double &alto);
	static bool valInclinacionObjeto(Json::Value obj, int &inclinacion);
	static bool valBasesObjeto(Json::Value obj, double &base_mayor, double &base_menor);
	static bool valEstaticoObjeto(Json::Value obj, bool &estatico);

	static std::string valColorObjeto(Json::Value obj);
	static int valRotObjeto(Json::Value obj);
	static int valMasaObjeto(Json::Value obj);

};

#endif /* PARSERVALIDATOR_H_ */
