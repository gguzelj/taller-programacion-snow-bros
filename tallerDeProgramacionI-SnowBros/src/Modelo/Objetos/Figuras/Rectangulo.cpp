#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){
	this->world->DestroyBody(this->body);
}
/*
 * ESte constructor se va a usar para todas las figuras rectangulo que vengan del json.
 */
Rectangulo::Rectangulo(JsonParser *parser, int index, b2World* world){
	this->type = "rectangulo";
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / (alto*ancho);
	fixture.friction = 0.7f;
	body->CreateFixture(&fixture);

}

float Rectangulo::getAncho(){
	return ancho;
}

float Rectangulo::getAlto(){
	return alto;
}
