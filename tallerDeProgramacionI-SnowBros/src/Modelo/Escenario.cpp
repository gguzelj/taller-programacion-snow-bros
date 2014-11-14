#include "../../headers/Modelo/Escenario.h"

int turnCooldown = 0;

Escenario::Escenario(JsonParser *parser) {

	Figura* figura_i;
	Rectangulo* muro_i;
	b2Vec2 gravity(0.0f, parser->getGravedad());

	cantidadMaximaDePersonajes = parser->getConnectionsLimit();
	figurasEstaticas_ = new std::vector<Figura*>;
	figurasDinamicas_ = new std::vector<Figura*>;
	proyectiles_ = new std::list<Proyectil*>;
	personajes_ = new std::list<Personaje*>;
	enemigos_ = new std::list<Enemigo*>;
	ancho_un = parser->getAnchoUnEscenario();
	alto_un = parser->getAltoUnEscenario();
	world_ = new b2World(gravity);

	world_->SetContactListener(&contactos);

	//Create the ground
	muro_i = new Rectangulo(80, 2, 0, 0, -20, world_);
	figurasEstaticas_->push_back(muro_i);
	//And walls
	muro_i = new Rectangulo(2, 42, 0, 41, 0, world_);
	figurasEstaticas_->push_back(muro_i);
	muro_i = new Rectangulo(2, 42, 0, -41, 0, world_);
	figurasEstaticas_->push_back(muro_i);

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

		else if (parser->getTipoObjeto(index) == ENEMIGOBASICO) {
			Enemigo* nuevoEnemigo = new Enemigo(parser, index, this); //todo puede haber error si no se crea cuidado!!
			enemigos_->push_back(nuevoEnemigo);
		}

		else
			throw ErrorTipoDeObjeto();

		if (parser->esObjetoEstatico(index))
			figurasEstaticas_->push_back(figura_i);
		else
			figurasDinamicas_->push_back(figura_i);
	}
}

Escenario::~Escenario() {

	for (int i = 0; figurasEstaticas_->size(); i++) {
		(figurasEstaticas_->pop_back());
	}
	for (int i = 0; figurasDinamicas_->size(); i++) {
		(figurasDinamicas_->pop_back());
	}
	for (int i = 0; personajes_->size(); i++) {
		(personajes_->pop_back());
	}

	delete figurasDinamicas_;
	delete figurasEstaticas_;
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
	for (auto per = personajes_->begin(); per != personajes_->end(); ++per) {
		if (ASIGNADO((*per)->id)) {
			strcpy((*per)->id, id);
			(*per)->setConnectionState(CONECTADO);
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
	for (auto personaje = personajes_->begin(); personaje != personajes_->end();
			++personaje) {
		if (strcmp((*personaje)->id, id) == 0)
			(*personaje)->setConnectionState(state);
	}
}

void destruirJointsDeBolaEnemigo(BolaEnemigo* enemigo,
		std::list<Personaje*>* personajes_, b2World* world_) {
	for (auto per = personajes_->begin(); per != personajes_->end(); ++per) {
		if (!ASIGNADO((*per)->id)) {
			if ((*per)->arrastradoPor == enemigo) {
				world_->DestroyJoint((*per)->joint);
				(*per)->arrastrado = false;
				(*per)->arrastradoPor = nullptr;
				(*per)->joint = nullptr;
				(*per)->state = &Personaje::standby;
			}
		}
	}
}

void Escenario::clean() {
	for (auto pro = proyectiles_->begin(); pro != proyectiles_->end(); ++pro) {
		b2Body* body = (*pro)->getb2Body();

		if ((*pro)->type == ID_BOLA_NIEVE_ENEMIGO) {
			if (!((BolaEnemigo*) (*pro))->destruir)
				continue;
		}

		for (b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next) {
			b2Contact* c = ce->contact;
			if (c->IsTouching()) {
				if ((*pro)->type == ID_BOLA_NIEVE_ENEMIGO) {
					destruirJointsDeBolaEnemigo((BolaEnemigo*) (*pro),
							personajes_, world_);
				}

				world_->DestroyBody((*pro)->getb2Body());
				proyectiles_->erase(pro++);
				break;
			}
		}
	}
}

void Escenario::preStep() {
	for (auto per = personajes_->begin(); per != personajes_->end(); ++per) {
		if (!ASIGNADO((*per)->id)) {
			(*per)->controlarEstado();
		}
	}

	for (auto en = enemigos_->begin(); en != enemigos_->end(); ++en) {
		if (!(*en)->estaVivo) {
			world_->DestroyBody((*en)->getb2Body());
			enemigos_->erase(en++);
		} else
			(*en)->controlarEstado();
	}
	clean();
}

void Escenario::step() {
	preStep();
	getWorld()->Step(timeStep, velocityIterations, positionIterations);
	actualizarEnemigos();
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

figura_t* Escenario::getObjetosEstaticos() {
	return getFiguras(figurasEstaticas_);
}

figura_t* Escenario::getObjetosDinamicos() {
	return getFiguras(figurasDinamicas_);
}

figura_t* Escenario::getFiguras(std::vector<Figura*>* vector) {

	figura_t* obj;
	Figura* fig;
	obj = (figura_t*) malloc(sizeof(figura_t) * vector->size());

	for (unsigned int i = 0; i < vector->size(); i++) {

		fig = (*vector)[i];

		obj[i].id = fig->getId();
		obj[i].alto = fig->getAlto();
		obj[i].ancho = fig->getAncho();
		obj[i].rotacion = fig->getAngulo();
		obj[i].centro.x = fig->GetCenter().x;
		obj[i].centro.y = fig->GetCenter().y;

	}
	return obj;
}

/*
 * Devolvemos un vector con proyectiles
 */
proyectil_t* Escenario::getProyectiles() {

	proyectil_t* p;
	p = (proyectil_t*) malloc(sizeof(proyectil_t) * proyectiles_->size());

	int i = 0;
	for (auto proy = proyectiles_->begin(); proy != proyectiles_->end();
			++proy) {

		p[i].id = (*proy)->getId();
		p[i].alto = (*proy)->getAlto();
		p[i].ancho = (*proy)->getAncho();
		p[i].rotacion = (*proy)->getAngulo();
		p[i].centro.x = (*proy)->GetCenter().x;
		p[i].centro.y = (*proy)->GetCenter().y;
		i++;
	}
	return p;
}

personaje_t* Escenario::getPersonajesParaEnvio() {
	personaje_t* pers = (personaje_t*) malloc(
			sizeof(personaje_t) * cantidadMaximaDePersonajes);

	unsigned int i = 0;
	for (auto personaje = personajes_->begin(); personaje != personajes_->end();
			++personaje) {
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
		pers[i].inmune = (*personaje)->getInmune();
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
		pers[i].inmune = true;
	}
	return pers;
}

enemigo_t* Escenario::getEnemigosParaEnvio() {
	enemigo_t* enems = (enemigo_t*) malloc(
			sizeof(enemigo_t) * enemigos_->size());

	int i = 0;
	for (auto enemigo = enemigos_->begin(); enemigo != enemigos_->end();
			++enemigo) {
		enems[i].alto = (*enemigo)->getAlto() * 2;
		enems[i].ancho = (*enemigo)->getAncho() * 2;
		enems[i].orientacion = (*enemigo)->getOrientacion();
		enems[i].estado = (*enemigo)->state->getCode();
		enems[i].centro.x = (*enemigo)->getX();
		enems[i].centro.y = (*enemigo)->getY();
		enems[i].nivelDeCongelacion = (*enemigo)->getNivelDeCongelamiento();
		i++;
	}
	return enems;
}

Personaje* Escenario::getPersonaje(conn_id id) {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end();
			++personaje) {
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

void Escenario::actualizarEnemigos() {

	for (auto en = enemigos_->begin(); en != enemigos_->end(); en++) {
		(*en)->mover();
	}
}
