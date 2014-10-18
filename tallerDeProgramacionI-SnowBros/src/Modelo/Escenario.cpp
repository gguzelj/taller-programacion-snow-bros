#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
	// Define the gravity vector and then create an instance of b2world
	b2Vec2 gravity(0.0f, parser->getGravedad());
	world_ = new b2World(gravity);
	figuras_ = new std::list<Figura*>;
	figurasEstaticas_ = new std::vector<Figura*>;
	figurasDinamicas_ = new std::vector<Figura*>;
	muros_ = new std::list<Muro*>;
	personajes_ = new std::list<Personaje*>;
	Figura* figura_i;
	Muro* muro_i;


	crearPersonaje(0,0,"nombre1");
	crearPersonaje(10,10,"nombre2");


	//Create the ground
	muro_i = new Muro(parser->getAnchoUnEscenario(),
			parser->getAltoUnEscenario(), 0, world_, 0);
	muros_->push_back(muro_i);
	//And walls
	muro_i = new Muro(parser->getAnchoUnEscenario(),
			parser->getAltoUnEscenario(), 90, world_,
			0.5 * parser->getAnchoUnEscenario());
	muros_->push_back(muro_i);
	muro_i = new Muro(parser->getAnchoUnEscenario(),
			parser->getAltoUnEscenario(), 90, world_,
			-0.5 * parser->getAnchoUnEscenario());
	muros_->push_back(muro_i);

	// Create all the objects
	for (unsigned int index = 0; index < parser->getCantidadObjetos();
			index++) {

		if (parser->getTipoObjeto(index) == CIRCULO)
			figura_i = new Circulo(parser, index, world_);

		else if (parser->getTipoObjeto(index) == RECTANGULO)
			figura_i = new Rectangulo(parser, index, world_);

		else if (parser->getTipoObjeto(index) == PARALELOGRAMO)
			figura_i = new Paralelogramo(parser, index, world_);

		else if (parser->getTipoObjeto(index) == POLIGONO)
			figura_i = new Poligono(parser, index, world_);

		else if (parser->getTipoObjeto(index) == TRAPECIO)
			figura_i = new Trapecio(parser, index, world_);

		else
			throw ErrorTipoDeObjeto();

		//Validamos si esta definicion del objeto no se superpone con alguna otra.
		//En ese caso, se elimina el objeto y se guarda en el log
		if (figura_i->validarOverlap()) {

			Log::instance()->append(
			FIGURA_WARNING_OVERLAP + parser->getObjectDefinition(index),
					Log::WARNING);
		} else {
			figuras_->push_back(figura_i);
			
			if (parser->esObjetoEstatico(index))
				figurasEstaticas_->push_back(figura_i);
				
			else
				figurasDinamicas_->push_back(figura_i);
		}
	}
}

Escenario::~Escenario() {
	// Borrar cada uno de los objetos
	for (int i = 0; figuras_->size(); i++) {
		(figuras_->pop_back());
	}
	for (int i = 0; personajes_->size(); i++) {
		(personajes_->pop_back());
	}

	// Borrar las listas
	delete figuras_;
	delete muros_;
	delete personajes_;
	delete world_;
}

b2World* Escenario::getWorld() {
	return world_;
}

std::list<Figura*>* Escenario::getFiguras() {
	return figuras_;
}

std::list<Personaje*>* Escenario::getPersonajes() {
	return personajes_;
}



bool Escenario::crearPersonaje(float x, float y,char id[]){
	Personaje* nuevoPersonaje = new Personaje(x,y,id,world_);
	if(!nuevoPersonaje)
		return false;
	personajes_->push_back(nuevoPersonaje);
	return true;
}



void acomodarEstadoPersonaje(Personaje* personaje){

	personaje->decreaseJumpCooldown();
	//chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
	//esta implementado aca para que cambie cuando tiene que hacerlo
	if (personaje->getVelocity().y <= 0.0f
			&& personaje->getCantidadDeContactosActuales() == 0) {
		personaje->state = &Personaje::falling;
	} else if (personaje->getVelocity().y <= 0.0f
			&& personaje->state == &Personaje::jumping) {
		personaje->state = &Personaje::standby;
	}

	if (Personaje::walking.movimientoLateralDerecha
			|| Personaje::walking.movimientoLateralIzquierda)
		Personaje::walking.caminar(*personaje);

	if (Personaje::jumping.debeSaltar && personaje->state->getCode() != JUMPING
			&& personaje->state->getCode() != FALLING) {
		personaje->jump();
		personaje->state = &Personaje::jumping;
	}

}



void Escenario::step() {

	list<Personaje*>::iterator iterador;
	iterador = personajes_->begin();
	while( iterador != personajes_->end())
	{
		acomodarEstadoPersonaje(*iterador);
		iterador++;
	}
	getWorld()->Step(timeStep, velocityIterations, positionIterations);

}

unsigned int Escenario::getCantObjDinamicos() {
	return figurasDinamicas_->size();
}

unsigned int Escenario::getCantObjEstaticos() {
	return figurasEstaticas_->size();
}

/**
 * Devolvemos un vector con objetos Estaticos
 */
objEstatico_t* Escenario::getObjetosEstaticos() {

	objEstatico_t* obj;
	Figura* fig;
	obj = (objEstatico*) malloc(
			sizeof(objEstatico_t) * figurasEstaticas_->size());

	for (unsigned int i = 0; i < figurasEstaticas_->size(); i++) {

		fig = (*figurasEstaticas_)[i];

		obj[i].alto = 9;
		obj[i].ancho = 9;
		obj[i].rotacion = fig->getAngulo();
		obj[i].centro.x = fig->x;
		obj[i].centro.y = fig->y;
	}

	return obj;

}

/**
 * Devolvemos un vector con objetos dinamicos
 */
objDinamico_t* Escenario::getObjetosDinamicos() {

	objDinamico_t* obj;
	Figura* fig;
	obj = (objDinamico*) malloc(
			sizeof(objDinamico_t) * figurasDinamicas_->size());

	for (unsigned int i = 0; i < figurasDinamicas_->size(); i++) {

		fig = (*figurasEstaticas_)[i];

		obj[i].alto = 9;
		obj[i].ancho = 9;
		obj[i].rotacion = fig->getAngulo();
		obj[i].centro.x = fig->x;
		obj[i].centro.y = fig->y;
	}

	return obj;

}