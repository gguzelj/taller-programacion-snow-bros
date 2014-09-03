#include "../headers/ventanaGrafica.h"
#include "../headers/Viewer.h"
#include "../headers/ObjectFactory.h"
#include "../headers/Drawer.h"
#include "../headers/JsonParser.h"

int main(int argc,char* argv[]){

	//Aca habria que cargar en memoria al JSON
	JsonParser *parser = new JsonParser();
	parser->test();

	//Inicializar la ventana
	VentanaGrafica* window = new VentanaGrafica();
	Viewer* viewer = window;

	//Inicializamos el ObjectFactory
	ObjectFactory* objectFactory = new ObjectFactory();

	//Inicializamos el Drawer
	Drawer* drawer = new Drawer();

	//Inicializamos el world del Box2D
	b2World* world = objectFactory->crearb2World();

	//Creamos las plataformas
	objectFactory->createPlatforms(world);

/*	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -13.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 7.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(1.0f, 4.0f);
	b2Body* body = world->CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.7f, 0.7f, bodyDef.position, 1.0);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction. Tocar y ver que pasa
	fixtureDef.friction = 0.5f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Define the static bodies. We set its position and call the body factory.
	b2BodyDef bodyDef1;
	bodyDef1.type = b2_staticBody;
	bodyDef1.position.Set(1.5f, 1.0f);
	b2Body* platform1 = world->CreateBody(&bodyDef1);

	b2PolygonShape staticBox;
	staticBox.SetAsBox(2.5f, 0.5f, bodyDef1.position, 0);

	b2FixtureDef fixtureDef1;
	fixtureDef1.shape = &staticBox;
	platform1->CreateFixture(&fixtureDef1);*/

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 2;

	// Game loop.
	int i = 0;
	while (i < 300){
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world->Step(timeStep, velocityIterations, positionIterations);

		SDL_RenderClear(window->renderer);

		(*viewer).reproducirVentana();
		drawer->drawScenary(world, window);

		SDL_RenderPresent(window->renderer);
		i++;
	}


	SDL_Delay(2000);
	delete viewer;
	delete world;

	return 0;
}
