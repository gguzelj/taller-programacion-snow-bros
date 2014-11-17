#include "../../../../headers/Modelo/Objetos/Personajes/EnemigoFuego.h"
#include "../../../../headers/Modelo/Escenario.h"

EnemigoFuego::EnemigoFuego(JsonParser *parser, int index, Escenario* escenario) :
Enemigo(parser, index, escenario)
{

	this->type = ID_ENEMIGO_FUEGO;

}

EnemigoFuego::~EnemigoFuego() {
	// TODO Auto-generated destructor stub
}

