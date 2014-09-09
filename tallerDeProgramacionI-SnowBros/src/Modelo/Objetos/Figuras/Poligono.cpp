#include "../../../../headers/Modelo/Objetos/Figuras/Poligono.h"

Poligono::Poligono(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	//this->lados = parser->getLados(index); TODO implementar getLados
	this->escala = parser->getEscalaObjeto(index); //Cambiar cuando el parser tenga el metodo getEscalaObjeto
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

std::list<std::pair<float, float>*>* Poligono::getListaVertices(){
	std::list<std::pair<float, float>*>* lista = new std::list<std::pair<float, float>*>;
//	int aux_x, aux_y;
//	aux_x = x + escala;
//	aux_y = y - escala;
//	for(int i = 0; i < lados; i++){
//		std::pair* par = new std::pair(aux_x, aux_y);
//		lista->push_front(par);
//
//	}
	return lista;
}
