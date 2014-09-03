#include "../headers/ObjectFactory.h"

ObjectFactory::ObjectFactory(){

}

ObjectFactory::~ObjectFactory(){

}


b2World* ObjectFactory::crearb2World(){
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);
	return new b2World(gravity);
}

/*
 * Por ahora creo una sola plataforma. Me parece que lo mejor es que las opciones default del escenario esten en algun archivo con formato json.
 */
void ObjectFactory::createPlatforms(b2World* world){
	//Eventualmente se llamara a crear varios objetos estaticos.
	createStaticBody(world);
}

void ObjectFactory::createStaticBody(b2World* world){
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody; //this will be a static body
	myBodyDef.position.Set(0.0f, 5.0f); //set the starting position
	myBodyDef.angle = 0; //set the starting angle

	//Esto me crea el cuerpo
	b2Body* staticBody = world->CreateBody(&myBodyDef);

	//Aca le damos la forma de rectangulo
	b2PolygonShape boxShape;
	boxShape.SetAsBox(2.5f, 0.5f);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	staticBody->CreateFixture(&boxFixtureDef);
}

