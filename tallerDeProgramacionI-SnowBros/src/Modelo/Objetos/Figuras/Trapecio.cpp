#include "../../../../headers/Modelo/Objetos/Figuras/Trapecio.h"

Trapecio::~Trapecio(){
	this->world->DestroyBody(this->body);
}

Trapecio::Trapecio(JsonParser *parser, int index, b2World* world){
	this->type = ID_TRAPECIO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->inclinacion = 90;
	this->alto = parser->getAltoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;
	this->teletransportar = false;

	if(inclinacion < 90){
		this->base_sup = parser->getBaseObjeto(index);
		this->base_inf = parser->getBaseObjeto(index)*0.68;
	} else{
		this->base_sup = parser->getBaseObjeto(index)*0.68;
		this->base_inf = parser->getBaseObjeto(index);

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
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / ((base_inf+base_sup)/2 * alto);
	fixtureDef.filter.groupIndex = FIGURA_FILTER_INDEX;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

float Trapecio::getBaseMayor(){
	if(inclinacion < 90)
		return this->base_sup;
	else
		return this->base_inf;
}

float Trapecio::getAlto(){
	return alto;
}

float Trapecio::getAncho(){
	return getBaseMayor();
}
char Trapecio::getId(){
	return TRAPECIO_CODE;
}
