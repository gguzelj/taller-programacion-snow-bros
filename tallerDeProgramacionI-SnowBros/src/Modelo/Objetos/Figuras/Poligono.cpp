#include "../../../../headers/Modelo/Objetos/Figuras/Poligono.h"

Poligono::Poligono(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->escala = escala; //Cambiar cuando el parser tenga el metodo getEscalaObjeto
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	//Despues ver como crear el b2Body

}

Poligono::~Poligono(){
	this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}
