#include "../../../../headers/Modelo/Objetos/Figuras/Rectangulo.h"

Rectangulo::~Rectangulo(){
	this->world->DestroyBody(this->body);
}
/*
 * ESte constructor se va a usar para todas las figuras rectangulo que vengan del json.
 */
Rectangulo::Rectangulo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->alto = parser->getAltoObjeto(index)*2;
	this->ancho = parser->getAnchoObjeto(index)*2;
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;

	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho/2,alto/2); 		//Esto es porque me hace el rectangulo el doble de ancho de lo que le pongo.
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / (alto*ancho);
	body->CreateFixture(&fixture);
}

/*
 * Este constructor se va a usar solamente para el piso
 */
Rectangulo::Rectangulo(JsonParser *parser, b2World* world){
	this->x = 0;
	float factorDeAmplificacion = (4.0f/7.0f);
	this->y = -factorDeAmplificacion * parser->getAltoUnEscenario();
	std::cout<<y<<std::endl;
	this->alto = 3;
	this->ancho = parser->getAnchoUnEscenario();
	this->masa = 1000;
	this->angulo = 0;
	this->estatico = true;
	this->color = "#000000";
	this->world = world;

	b2BodyDef cuerpo;
	cuerpo.type = b2_staticBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x,y);

	b2Body* body = this->world->CreateBody(&cuerpo);
	this->body = body;

	//definiendo ahora el fixture del rectangulo
	b2PolygonShape shape;
	shape.SetAsBox(ancho,alto);
	b2FixtureDef fixture;					//creo el fixture
	fixture.shape = &shape;					//le asigno la forma que determine antes
	fixture.density = (float) masa / (alto*ancho);
	body->CreateFixture(&fixture);
}
