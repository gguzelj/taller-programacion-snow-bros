#include "../../../../headers/Modelo/Objetos/Figuras/Trapecio.h"

Trapecio::~Trapecio(){
	this->world->DestroyBody(this->body);
}

Trapecio::Trapecio(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->inclinacion = parser->getInclinacionObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->color = parser->getColorObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	if(inclinacion < 90){
		this->base_sup = parser->getBaseMayorObjeto(index);
		this->base_inf = parser->getBaseMenorObjeto(index);
	} else{
		this->base_sup = parser->getBaseMenorObjeto(index);
		this->base_inf = parser->getBaseMayorObjeto(index);

	}

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	this->body = this->world->CreateBody(&cuerpo);

	//Definiendo ahora el fixture del Trapecio
	float mediana = (base_inf + base_sup) / 2;
	float hip = alto / sin(inclinacion * DEGTORAD);

	b2Vec2 vertices[4];
	vertices[0].Set( mediana / 2 + cos(inclinacion * DEGTORAD) * hip / 2, alto / 2);	//Superior der
	vertices[1].Set( mediana / 2 - cos(inclinacion * DEGTORAD) * hip / 2, -alto / 2);	//Inferior der
	vertices[2].Set(vertices[0].x - base_sup, vertices[0].y);							//Superior izq
	vertices[3].Set(vertices[1].x - base_inf, vertices[1].y);							//Inferior izq

	b2PolygonShape shape;
	shape.Set(vertices,4);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / ((base_inf+base_sup)/2 * alto);
	fixture.friction = 1.0f;
	body->CreateFixture(&fixture);

}
