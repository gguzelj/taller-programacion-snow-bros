#include "../../../../headers/Modelo/Objetos/Figuras/PlataformaHielo.h"

PlataformaHielo::PlataformaHielo(JsonParser *parser, int index, b2World* world){
	this->type = ID_PLATAFORMAHIELO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index);
	this->ancho = parser->getAnchoObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;
	this->teletransportar = false;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);
	body = world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = (float) masa / (alto*ancho);
	fixtureDef.filter.groupIndex = FIGURA_FILTER_INDEX;
	fixtureDef.friction = 0;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
}

PlataformaHielo::~PlataformaHielo(){
	world->DestroyBody(body);
}

void PlataformaHielo::beginContact(Figura* figura,b2Contact* contact){
	figura->beginContactPlataformaHielo(this, contact);
}

float PlataformaHielo::getAncho(){
	return ancho;
}

float PlataformaHielo::getAlto(){
	return alto;
}

char PlataformaHielo::getId(){
	return PLATAFORMAHIELO_CODE;
}
