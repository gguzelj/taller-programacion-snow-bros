#include "../../../../headers/Modelo/Objetos/Figuras/Poligono.h"

Poligono::~Poligono() {
	this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}

Poligono::Poligono(JsonParser *parser, int index, b2World* world) {
	this->type = ID_POLIGONO;
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->lados = parser->getLadosObjeto(index);
	this->escala = parser->getEscalaObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index) * DEGTORAD;
	this->estatico = parser->esObjetoEstatico(index);
	this->world = world;

	b2BodyDef cuerpo;
	estatico ? cuerpo.type = b2_staticBody : cuerpo.type = b2_dynamicBody;
	cuerpo.angle = angulo;
	cuerpo.position.Set(x, y);
	this->body = this->world->CreateBody(&cuerpo);

	//definiendo ahora el fixture del poligono
	b2Vec2 vertices[this->lados];
	int numVertices = this->lados;

	float angle = 2 * M_PI / numVertices;
	for (int i = 0; i < numVertices; i++) {
		float pointX = this->escala * sin(i * angle);
		float pointY = this->escala * cos(i * angle);
		vertices[i].Set(pointX, pointY);
	}

	b2PolygonShape shape;
	shape.Set(vertices, numVertices);
	b2FixtureDef fixtureDef;	//creo el fixture
	fixtureDef.shape = &shape;	//le asigno la forma que determine antes
	float area = pow(this->escala, 2) * numVertices * tan(M_PI / numVertices);
	fixtureDef.density = masa / area;
	fixtureDef.friction = 0.7f;

	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);

}

int Poligono::getLados() {
	return lados;
}

float Poligono::getEscala() {
	return escala;
}

float Poligono::getAlto() {

	switch (lados) {
	case 3:
		return escala * 3 / 2;
	case 4:
		return 2 * escala;
	case 5:
		return escala * (1 + cos(M_PI / 5));
	case 6:
		return 2 * escala;
	}

	return 0;

}

float Poligono::getAncho() {

	switch (lados) {
	case 3:
		return 1.732050808 * escala;
	case 4:
		return 2 * escala;
	case 5:
		return (2 * escala * sin(M_PI / 5) * (1 + 2 * cos(72 * DEGTORAD)));
	case 6:
		return (2 * escala * cos(M_PI / 6));
	}

	return 0;

}

char Poligono::getId() {

	switch (lados) {
	case 3:
		return TRIANGULO_CODE;
	case 4:
		return CUADRADO_CODE;
	case 5:
		return PENTAGONO_CODE;
	case 6:
		return HEXAGONO_CODE;
	}

	return HEXAGONO_CODE;

}
