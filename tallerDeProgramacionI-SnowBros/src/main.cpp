#include "../headers/ventanaGrafica.h"
#include "../headers/ObjectFactory.h"
#include "../headers/Drawer.h"
#include "../headers/JsonParser.h"
#include "../headers/GestorDeEventos.h"

int main(int argc,char* argv[]){
	bool quit,reset;
	do{
		reset = false;
		GestorDeEventos* g_eventos = new GestorDeEventos();

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
		quit = false;
		SDL_Event evento;
		while (!quit){

			//Manejo de eventos
			SDL_WaitEvent(&evento);
			g_eventos->interpretarEvento(evento,&quit,&reset);

			// Instruct the world to perform a single step of simulation.
			// It is generally best to keep the time step and iterations fixed.
			world->Step(timeStep, velocityIterations, positionIterations);

			drawer->clearScenary();
			drawer->drawBackground();
			drawer->drawScenary(world);
			drawer->presentScenary();

			SDL_Delay(2);
		}



		delete parser;
		delete g_eventos;
		delete window;
		delete world;
		for(int i=1;i<2000;i++){
			wait();
		}
	}while(reset);

	return 0;
}
