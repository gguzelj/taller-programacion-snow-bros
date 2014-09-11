#include "../../../../headers/Modelo/Objetos/Figuras/Trapecio.h"

Trapecio::~Trapecio(){
	this->world->DestroyBody(this->body);
}

Trapecio::Trapecio(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->inclinacion = 30; //parser->getInclinacionObjeto(index); no me esta leyendo bien la inclinacion del parser.
	this->alto = parser->getAltoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->color = parser->getColorObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

//	if(inclinacion < 90){
//		this->base_inf = parser->getBaseMayorObjeto(index);
//		this->base_sup = parser->getBaseMenorObjeto(index);
//	} else{
//		this->base_inf = parser->getBaseMenorObjeto(index);
//		this->base_sup = parser->getBaseMayorObjeto(index);
//
//	}

	this->base_inf = 3; //Por ahora
	this->base_sup = 5;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//Definiendo ahora el fixture del Trapecio

	float mediana = (base_inf + base_sup) / 2;
	float hip = alto / cos(inclinacion * DEGTORAD);

	b2Vec2 vertices[4];
	//vertices[0].Set(x + mediana / 2 + cos((float)(180-inclinacion) * DEGTORAD) * hip / 2, y - alto / 2);	//Inf der
	//vertices[1].Set(x + mediana / 2 - cos((float)(180-inclinacion) * DEGTORAD) * hip / 2, y + alto / 2);	//Sup der
	vertices[0].Set(0, 0);
	vertices[1].Set(1.5, 1.5);
	vertices[2].Set(vertices[1].x - base_sup, vertices[1].y);											//Sup izq
	vertices[3].Set(vertices[0].x - base_inf, vertices[0].y);											//Inf izq

	b2PolygonShape shape;
	shape.Set(vertices,4);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / ((base_inf+base_sup)/2 * alto);
	body->CreateFixture(&fixture);

}
