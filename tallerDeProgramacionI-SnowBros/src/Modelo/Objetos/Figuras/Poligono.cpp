#include "../../../../headers/Modelo/Objetos/Figuras/Poligono.h"

Poligono::Poligono(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->lados = parser->getLadosObjeto(index);
	this->escala = parser->getEscalaObjeto(index);
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

	//definiendo ahora el fixture del poligono
	b2Vec2 vertices[this->lados];
	int numVertices = this->lados;

	float angle = 2 * M_PI / numVertices;
	for (int i = 0; i < numVertices; i++)
	{
	    float pointX = this->x + this->escala * sin(i * angle);
	    float pointY = this->y + this->escala * cos(i * angle);
	    vertices[i].Set(pointX,pointY);
	    std::cout << "x: " << vertices[i].x << " y: " << vertices[i].y << std::endl;
	}

	b2PolygonShape shape;
	shape.Set(vertices,numVertices);
	b2FixtureDef fixture;	//creo el fixture
	fixture.shape = &shape;	//le asigno la forma que determine antes
	float area = pow(this->escala,2) * numVertices * tan(M_PI/numVertices);
	fixture.density = masa / area;
	body->CreateFixture(&fixture);

}

Poligono::~Poligono(){
	this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}
