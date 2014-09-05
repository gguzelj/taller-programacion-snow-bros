#include "../headers/ventanaGrafica.h"
#include "../headers/ObjectFactory.h"
#include "../headers/Drawer.h"
#include "../headers/JsonParser.h"

int main(int argc,char* argv[]){

	JsonParser *parser = new JsonParser();
	parser->test();

	//Inicializar la ventana
	VentanaGrafica* window = new VentanaGrafica();
	Drawer* drawer = window;

	//Inicializamos el ObjectFactory
	ObjectFactory* objectFactory = new ObjectFactory();

	//Inicializamos el world del Box2D
	b2World* world = objectFactory->crearb2World();

	//Creamos las plataformas
	objectFactory->createPlatforms(world);

	//Parametros del step
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 2;

	// Game loop.
	int i = 0;
	while (i < 300){
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world->Step(timeStep, velocityIterations, positionIterations);

		drawer->clearScenary();
		drawer->drawBackground();
		drawer->drawScenary(world);
		drawer->presentScenary();

		i++;
	}


	SDL_Delay(2000);
	delete drawer;
	delete world;

	return 0;
}
