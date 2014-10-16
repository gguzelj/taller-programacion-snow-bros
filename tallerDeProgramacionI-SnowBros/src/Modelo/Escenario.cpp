#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
	// Define the gravity vector and then create an instance of b2world
	b2Vec2 gravity(0.0f, parser->getGravedad());
	world_ = new b2World(gravity);
	figuras_ = new std::list<Figura*>;
	muros_ = new std::list<Muro*>;
	Figura* figura_i;
	Muro* muro_i;

	// Create the guy
	person_ = new Personaje(parser, world_);

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
		}
	}
}

Escenario::~Escenario() {
	// Borrar cada uno de los objetos
	for (int i = 0; figuras_->size(); i++) {
		(figuras_->pop_back());
	}

	// Borrar las listas
	delete figuras_;
	delete muros_;
	delete person_;
	delete world_;
}

b2World* Escenario::getWorld() {
	return world_;
}

std::list<Figura*>* Escenario::getFiguras() {
	return figuras_;
}

Personaje* Escenario::getPersonaje() {
	return person_;
}

void Escenario::step() {

	person_->decreaseJumpCooldown();
	//chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
	//esta implementado aca para que cambie cuando tiene que hacerlo
	if (person_->getVelocity().y <= 0.0f
			&& person_->getCantidadDeContactosActuales() == 0) {
		person_->state = &Personaje::falling;
	} else if (person_->getVelocity().y <= 0.0f
			&& person_->state == &Personaje::jumping) {
		person_->state = &Personaje::standby;
	}

	if (Personaje::walking.movimientoLateralDerecha
			|| Personaje::walking.movimientoLateralIzquierda)
		Personaje::walking.caminar(*(getPersonaje()));

	if (Personaje::jumping.debeSaltar && person_->state->getCode() != JUMPING
			&& person_->state->getCode() != FALLING) {
		person_->jump();
		person_->state = &Personaje::jumping;
	}
	getWorld()->Step(timeStep, velocityIterations, positionIterations);
}
