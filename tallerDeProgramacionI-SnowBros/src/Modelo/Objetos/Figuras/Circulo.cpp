#include "../../../../headers/Modelo/Objetos/Figuras/Circulo.h"

Circulo::Circulo(JsonParser *parser, int index, b2World* world){
	this->x = parser->getCoorXObjeto(index);
	this->y = parser->getCoorYObjeto(index);
	this->radio = parser->getEscalaObjeto(index);
	this->masa = parser->getMasaObjeto(index);
	this->angulo = parser->getRotObjeto(index);
	this->estatico = parser->esObjetoEstatico(index);
	this->color = parser->getColorObjeto(index);
	this->world = world;

	//Defino el body y fixture en comun
	b2BodyDef cuerpoDeCirculo;
	estatico ? cuerpoDeCirculo.type = b2_staticBody : cuerpoDeCirculo.type = b2_dynamicBody; //veo que tipo le asigno dependiendo de que se pida
	b2FixtureDef fixture1;                          //creo el fixture
	fixture1.density = masa / (radio*radio*M_PI);
	b2FixtureDef fixture2;                          //creo el fixture
	fixture2.density = 0.000000001;
	cuerpoDeCirculo.position.Set(x,y);

	//SHapes
	int numVertices = 6;
	b2Vec2 vertices[numVertices];
	float angle = 2 * M_PI / numVertices;
	for (int i = 0; i < numVertices; i++){
		float pointX = this->radio/2 * sin(i * angle);
		float pointY = this->radio/2 * cos(i * angle);
		vertices[i].Set(pointX,pointY);
	}
	b2PolygonShape tuerca;
	tuerca.Set(vertices,numVertices);
	b2CircleShape circleShape;
	circleShape.m_p.Set(0,0);
	circleShape.m_radius = radio;

	fixture1.shape = &circleShape;
	fixture1.friction = 0.1f;
	fixture2.shape = &tuerca;
	this->bodyInterno = this->world->CreateBody(&cuerpoDeCirculo);
	this->body = this->world->CreateBody(&cuerpoDeCirculo);
	body->CreateFixture(&fixture2);
	this->body->CreateFixture(&fixture1);

	//Defino el revolute joint para que ambos bodies roten sobre un mismo cir
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = this->body;
	revoluteJointDef.bodyB = this->bodyInterno;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(0,0); //The center of the circle
	revoluteJointDef.localAnchorB.Set(0,0); //The center of the polygon
	revoluteJointDef.referenceAngle = 0; //The reference angle between the two bodies
	this->world->CreateJoint(&revoluteJointDef);
}

Circulo::~Circulo(){
        this->world->DestroyBody(this->body); //tener cuidado con esto, no usarlo despues
}

float Circulo::getRadio(){
        return radio;
}

float Circulo::getCoordX(){
        b2Vec2 vec = this->body->GetPosition();
        return vec.x;
}

float Circulo::getCoordY(){
        b2Vec2 vec = this->body->GetPosition();
        return vec.y;
}
