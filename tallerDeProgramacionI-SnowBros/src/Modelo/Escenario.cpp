#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
        // Define the gravity vector and then create an instance of b2world
        b2Vec2 gravity(0.0f, -10.0f);
        world_ = new b2World(gravity);

        figuras_ = new std::list<Figura*>;
        // Create all the objects
        for(unsigned int index = 0; index < parser->getCantidadObjetos(); index++){
			Figura* figura_i;
			if(parser->getTipoObjeto(index) == "circulo")
				figura_i = new Circulo(parser,index, world_);

			else if(parser->getTipoObjeto(index) == "rectangulo")
				figura_i = new Rectangulo(parser,index, world_);

			else if(parser->getTipoObjeto(index) == "paralelogramo")
				figura_i = new Paralelogramo(parser,index, world_);

			else if(parser->getTipoObjeto(index) == "poligono")
				figura_i = new Poligono(parser,index, world_);

			else throw; //Esto para mi hay que cambiarlo. En vez de lanzar una excepcion, deberia seguir el flujo del programa ignorando
						//el error, pero guardandolo en el log de errores.

			figuras_->push_back(figura_i);

        }

        // Create the guy
        person_ = new Personaje(parser, world_);
}

Escenario::~Escenario() {
        delete world_;

        // Borrar cada uno de los objetos
        for (int i=0; figuras_->size();i++){
        	(figuras_->pop_back());
        }

        // Borrar la lista
        delete figuras_;
        delete person_;
}

b2World* Escenario::getWorld(){
	return world_;
}

std::list<Figura*>* Escenario::getFiguras(){
        return figuras_;
}

Personaje* Escenario::getPersonaje(){
        return person_;
}
