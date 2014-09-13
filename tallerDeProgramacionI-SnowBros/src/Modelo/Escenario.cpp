#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
        // Define the gravity vector and then create an instance of b2world
        b2Vec2 gravity(0.0f, -10.0f);
        world_ = new b2World(gravity);
        figuras_ = new std::list<Figura*>;
        muros_ = new std::list<Muro*>;
        Figura* figura_i;
        Muro* muro_i;

        //Create the ground
        muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 0, world_, 0);
        muros_->push_back(muro_i);
        //And walls
        muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 90, world_, (1.0f/2.0f)*parser->getAnchoUnEscenario());
        muros_->push_back(muro_i);
        muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 90, world_,- (1.0f/2.0f)*parser->getAnchoUnEscenario());
        muros_->push_back(muro_i);


        // Create all the objects
        for(unsigned int index = 0; index < parser->getCantidadObjetos(); index++){

        	if(parser->getTipoObjeto(index) == CIRCULO)
				figura_i = new Circulo(parser, index, world_);

			else if(parser->getTipoObjeto(index) == RECTANGULO)
				figura_i = new Rectangulo(parser, index, world_);

			else if(parser->getTipoObjeto(index) == PARALELOGRAMO)
				figura_i = new Paralelogramo(parser, index, world_);

			else if(parser->getTipoObjeto(index) == POLIGONO)
				figura_i = new Poligono(parser, index, world_);

			else if(parser->getTipoObjeto(index) == TRAPECIO)
				figura_i = new Trapecio(parser, index, world_);

			else throw; //Esto para mi hay que cambiarlo. En vez de lanzar una excepcion, deberia seguir el flujo del programa ignorando
						//el error, pero guardandolo en el log de errores.

			figuras_->push_back(figura_i);
        }

        // Create the guy
        //person_ = new Personaje(parser, world_);
}

Escenario::~Escenario() {
        // Borrar cada uno de los objetos
        for (int i=0; figuras_->size();i++){
        	(figuras_->pop_back());
        }

        // Borrar la lista
        delete figuras_;
        delete muros_;
        //delete person_; todavia no estamos instanciando a la persona.
        delete world_;
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
