#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
	// Define the gravity vector and then create an instance of b2world
	b2Vec2 gravity(0.0f, parser->getGravedad());
	ancho_un = parser->getAnchoUnEscenario();
	alto_un = parser->getAltoUnEscenario();
	world_ = new b2World(gravity);

	world_->SetContactListener(&contactos);

	figurasEstaticas_ = new std::vector<Figura*>;
	figurasDinamicas_ = new std::vector<Figura*>;
	proyectiles_ = new std::list<Proyectil*>;
	muros_ = new std::list<Muro*>;
	personajes_ = new std::list<Personaje*>;
	enemigos_ = new std::list<Enemigo*>;

	crearEnemigo(10,10);
	crearEnemigo(20,20);


	cantidadMaximaDePersonajes = parser->getConnectionsLimit();
	//TODO esto deberia ser la longitud del arreglo de enemigos del parser.
	cantidadMaximaDeEnemigos = 10;
	contactos.setPersonaje(personajes_);
	contactos.setEnemigos(enemigos_);
	contactos.setCharacters();

	Figura* figura_i;
	Muro* muro_i;

	//Create the ground
	muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 0, world_, 0);
	muros_->push_back(muro_i);
	//And walls
	muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 90, world_, 0.5 * parser->getAnchoUnEscenario());
	muros_->push_back(muro_i);
	muro_i = new Muro(parser->getAnchoUnEscenario(), parser->getAltoUnEscenario(), 90, world_, -0.5 * parser->getAnchoUnEscenario());
	muros_->push_back(muro_i);

	// Create all the objects
	for (unsigned int index = 0; index < parser->getCantidadObjetos(); index++) {

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

			Log::ins()->add(
			FIGURA_WARNING_OVERLAP + parser->getObjectDefinition(index), Log::WARNING);
		} else {
			if (parser->esObjetoEstatico(index))
				figurasEstaticas_->push_back(figura_i);
			else
				figurasDinamicas_->push_back(figura_i);
		}
	}
}

Escenario::~Escenario() {
	// Borrar cada uno de los objetos
	for (int i = 0; figurasEstaticas_->size(); i++) {
		(figurasEstaticas_->pop_back());
	}
	for (int i = 0; figurasDinamicas_->size(); i++) {
		(figurasDinamicas_->pop_back());
	}
	for (int i = 0; personajes_->size(); i++) {
		(personajes_->pop_back());
	}

	// Borrar las listas
	delete muros_;
	delete personajes_;
	delete world_;
}

b2World* Escenario::getWorld() {
	return world_;
}

std::list<Personaje*>* Escenario::getPersonajes() {
	return personajes_;
}

bool Escenario::asignarPersonaje(conn_id id) {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		if (strcmp((*personaje)->id, "sin asignar") == 0) {
			strcpy((*personaje)->id, id);
			(*personaje)->setConnectionState(CONECTADO);
			return true;
		}
	}
	return false;
}

bool Escenario::crearPersonaje(float x, float y, conn_id id) {
	Personaje* nuevoPersonaje = new Personaje(x, y, id, this);
	if (!nuevoPersonaje)
		return false;
	personajes_->push_back(nuevoPersonaje);
	return true;
}

void Escenario::setPersonajeConnectionState(conn_id id, char state) {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		if (strcmp((*personaje)->id, id) == 0)
			(*personaje)->setConnectionState(state);
	}
}

void acomodarEstadoCharacter(Character* personaje) {
	personaje->decreaseJumpCooldown();
	//chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
	//esta implementado aca para que cambie cuando tiene que hacerlo
	if (personaje->getVelocity().y <= 0.0f && personaje->getCantidadDeContactosActuales() == 0 && personaje->state != &Personaje::shooting) {
		personaje->state = &Personaje::falling;
	} else if (personaje->getVelocity().y <= 0.0f && personaje->state == &Personaje::jumping) {
		personaje->state = &Personaje::standby;
	}

	if (personaje->movimientoLateralDerecha || personaje->movimientoLateralIzquierda)
		Personaje::walking.caminar(*personaje);

	if (personaje->debeSaltar && personaje->state->getCode() != JUMPING && personaje->state->getCode() != FALLING) {
		personaje->jump();
		personaje->state = &Personaje::jumping;
	}

}

void Escenario::step() {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		if (strcmp((*personaje)->id, "sin asignar") != 0)
			acomodarEstadoCharacter(*personaje);
	}

	for (auto enemigo= enemigos_->begin(); enemigo != enemigos_->end(); ++enemigo) {
					acomodarEstadoCharacter(*enemigo);
	}

	for (auto proy = proyectiles_->begin(); proy != proyectiles_->end(); ++proy){
		b2Body* body = (*proy)->getb2Body();
		for( b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next ){
			b2Contact* c = ce->contact;
			if(c->IsTouching()){
				world_->DestroyBody((*proy)->getb2Body());
				proyectiles_->erase(proy++);
				break;
			}
		}
	}
	getWorld()->Step(timeStep, velocityIterations, positionIterations);
}

unsigned int Escenario::getCantPersonajes() {
	return cantidadMaximaDePersonajes;
}
unsigned int Escenario::getCantEnemigos() {
	return enemigos_->size();
}

unsigned int Escenario::getCantProyectiles() {
	return proyectiles_->size();
}

unsigned int Escenario::getCantObjDinamicos() {
	return figurasDinamicas_->size();
}

unsigned int Escenario::getCantObjEstaticos() {
	return figurasEstaticas_->size();
}

void obtenerAltoAnchoIdFigura(Figura* figura, float &alto, float &ancho, char& id) {

	if (figura->type == "rectangulo") {
		Rectangulo* rect = static_cast<Rectangulo *>(figura);
		alto = rect->getAlto();
		ancho = rect->getAncho();
		id = RECTANGULO_CODE;
		return;
	}
	if (figura->type == "circulo") {
		Circulo* circ = static_cast<Circulo *>(figura);
		alto = circ->getRadio() * 2;
		ancho = alto;
		id = CIRCULO_CODE;
		return;
	}

	if (figura->type == "poligono") {
		Poligono* polygon = static_cast<Poligono *>(figura);
		float escala = polygon->getEscala();
		int lados = polygon->getLados();

		if (lados == 3) {
			ancho = 1.732050808 * escala;
			alto = escala * 3 / 2;
			id = TRIANGULO_CODE;
			return;
		}

		if (lados == 4) {
			ancho = 2 * escala;
			alto = ancho;
			id = CUADRADO_CODE;
			return;
		}

		if (lados == 5) {
			ancho = (2 * escala * sin(M_PI / 5) * (1 + 2 * cos(72 * DEGTORAD)));
			alto = escala * (1 + cos(M_PI / 5));
			id = PENTAGONO_CODE;
			return;
		}

		if (lados == 6) {
			ancho = 2 * escala * cos(M_PI / 6);
			alto = 2 * escala;
			id = HEXAGONO_CODE;
			return;
		}
	}

	if (figura->type == "trapecio") {
		Trapecio* trap = static_cast<Trapecio *>(figura);
		ancho = trap->getBaseMayor();
		alto = trap->getAlto();
		id = TRAPECIO_CODE;
		return;
	}

	if (figura->type == "paralelogramo") {
		Paralelogramo* paralelogramo = static_cast<Paralelogramo *>(figura);

		alto = paralelogramo->getAlto();
		ancho = paralelogramo->getAncho() + alto / tan(M_PI / 6);
		id = PARALELOGRAMO_CODE;
		return;
	}
}

void obtenerAltoAnchoIdProyectil(Proyectil* proy, float &alto, float &ancho, char& id) {

	if (proy->type == "bolaNieve") {
		BolaNieve* bola = static_cast<BolaNieve *>(proy);
		alto = bola->getRadio() * 2;
		ancho = alto;
		id = BOLA_NIEVE_CODE;
		return;
	}

}

/**
 * Devolvemos un vector con objetos Estaticos
 */
figura_t* Escenario::getObjetosEstaticos() {

	figura_t* obj;
	Figura* fig;
	obj = (figura_t*) malloc(sizeof(figura_t) * figurasEstaticas_->size());

	for (unsigned int i = 0; i < figurasEstaticas_->size(); i++) {

		fig = (*figurasEstaticas_)[i];

		obtenerAltoAnchoIdFigura(fig, obj[i].alto, obj[i].ancho, obj[i].id);

		obj[i].rotacion = fig->getAngulo();
		b2Vec2 center = fig->GetCenter();
		obj[i].centro.x = center.x;
		obj[i].centro.y = center.y;
	}
	return obj;
}

/**
 * Devolvemos un vector con objetos dinamicos
 */
figura_t* Escenario::getObjetosDinamicos() {

	figura_t* obj;
	Figura* fig;
	obj = (figura_t*) malloc(sizeof(figura_t) * figurasDinamicas_->size());

	for (unsigned int i = 0; i < figurasDinamicas_->size(); i++) {

		fig = (*figurasDinamicas_)[i];

		obtenerAltoAnchoIdFigura(fig, obj[i].alto, obj[i].ancho, obj[i].id);

		obj[i].rotacion = fig->getAngulo();
		b2Vec2 center = fig->GetCenter();
		obj[i].centro.x = center.x;
		obj[i].centro.y = center.y;
	}
	return obj;
}

/**
 * Devolvemos un vector con proyectiles
 */
proyectil_t* Escenario::getProyectiles() {

	proyectil_t* p;
	p = (proyectil_t*) malloc(sizeof(proyectil_t) * proyectiles_->size());

	int i = 0;
	for (auto proy = proyectiles_->begin(); proy != proyectiles_->end(); ++proy){

		obtenerAltoAnchoIdProyectil((*proy), p[i].alto, p[i].ancho, p[i].id);

		p[i].rotacion = (*proy)->getAngulo();
		b2Vec2 center = (*proy)->GetCenter();
		p[i].centro.x = center.x;
		p[i].centro.y = center.y;
		i++;
	}
	return p;
}

personaje_t* Escenario::getPersonajesParaEnvio() {
	personaje_t* pers = (personaje_t*) malloc(sizeof(personaje_t) * cantidadMaximaDePersonajes);

	unsigned int i = 0;
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		pers[i].alto = (*personaje)->getAlto();
		pers[i].ancho = (*personaje)->getAncho();
		strcpy(pers[i].id, (*personaje)->id);
		pers[i].orientacion = (*personaje)->getOrientacion();
		pers[i].estado = (*personaje)->state->getCode();
		pers[i].centro.x = (*personaje)->getX();
		pers[i].centro.y = (*personaje)->getY();
		pers[i].connectionState = (*personaje)->getConnectionState();
		pers[i].points = (*personaje)->getPoints();
		pers[i].lives = (*personaje)->getLives();
		i++;
	}

	//Rellena con basura
	for (; i < cantidadMaximaDePersonajes; i++) {
		pers[i].alto = 0;
		pers[i].ancho = 0;
		strcpy(pers[i].id, "no");
		pers[i].orientacion = 'n';
		pers[i].estado = 'n';
		pers[i].centro.x = 0;
		pers[i].centro.y = 0;
		pers[i].connectionState = DESCONECTADO;
	}

	return pers;
}

enemigo_t* Escenario::getEnemigosParaEnvio() {
	enemigo_t* enems = (enemigo_t*) malloc(sizeof(enemigo_t) * cantidadMaximaDeEnemigos);

	int i = 0;
	for (auto enemigo = enemigos_->begin(); enemigo != enemigos_->end(); ++enemigo) {
		enems[i].alto = (*enemigo)->getAlto()*2;
		enems[i].ancho = (*enemigo)->getAncho()*2;
		enems[i].orientacion = (*enemigo)->getOrientacion();
		enems[i].estado = (*enemigo)->state->getCode();
		enems[i].centro.x = (*enemigo)->getX();
		enems[i].centro.y = (*enemigo)->getY();
		i++;
	}

	return enems;
}

Personaje* Escenario::getPersonaje(conn_id id) {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		if (strcmp((*personaje)->id, id) == 0)
			return *personaje;
	}
	return nullptr;
}

float Escenario::getAnchoUn() {
	return this->ancho_un;
}

void Escenario::agregarProyectil(Proyectil* proy) {
	proyectiles_->push_back(proy);
}


void Escenario::actualizarEnemigos(){
	for(auto enemigo = enemigos_->begin();enemigo != enemigos_->end();enemigo++){
		int v1 = rand() % 100;
		if(v1 <= 25)
			(*enemigo)->handleInput(SDLK_LEFT,SDL_KEYDOWN);
		else{
			if(v1 <= 50)
				(*enemigo)->handleInput(SDLK_RIGHT,SDL_KEYDOWN);
			else{
				if(v1 <= 75)
					(*enemigo)->handleInput(SDLK_RIGHT,SDL_KEYUP);
				else
					(*enemigo)->handleInput(SDLK_LEFT,SDL_KEYUP);
			}

		}
	}
}

bool Escenario::crearEnemigo(float x, float y) {
	Enemigo* nuevoEnemigo = new Enemigo(x, y, this->world_);
	if (!nuevoEnemigo)
		return false;
	enemigos_->push_back(nuevoEnemigo);
	return true;
}
