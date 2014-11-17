#include "../../../../headers/Modelo/Objetos/Personajes/EnemigoBasico.h"
#include "../../../../headers/Modelo/Escenario.h"

EnemigoBasico::EnemigoBasico(JsonParser *parser, int index, Escenario* escenario) :
Enemigo(parser, index, escenario)
{

	this->type = ID_ENEMIGO_BASICO;

}

EnemigoBasico::~EnemigoBasico() {
	// TODO Auto-generated destructor stub
}

