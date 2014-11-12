#include "../../headers/Modelo/Escenario.h"

int turnCooldown = 0;

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
	personajes_ = new std::list<Personaje*>;
	enemigos_ = new std::list<Enemigo*>;

	crearEnemigo(10, 10);
	crearEnemigo(20, 20);

	cantidadMaximaDePersonajes = parser->getConnectionsLimit();

	contactos.setPersonaje(personajes_);
	contactos.setEnemigos(enemigos_);
	contactos.setCharacters();

	Figura* figura_i;
	Rectangulo* muro_i;

	//Create the ground
	muro_i = new Rectangulo(80, 2, 0, 0, -20, world_);
	figurasEstaticas_->push_back(muro_i);
	//And walls
	muro_i = new Rectangulo(2, 42, 0, 41, 0, world_);
	figurasEstaticas_->push_back(muro_i);
	muro_i = new Rectangulo(2, 42, 0, -41, 0, world_);
	figurasEstaticas_->push_back(muro_i);

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

		if (parser->esObjetoEstatico(index))
			figurasEstaticas_->push_back(figura_i);
		else
			figurasDinamicas_->push_back(figura_i);
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
	if(personaje->state != &Personaje::dying){
		//Chequeo para cambiar el estado jumping a falling o el estado cuando cae de una plataforma
		personaje->decreaseJumpCooldown();
		//Chequeo si puedo disparar.
		personaje->decreaseShootCooldown();
		//Esta implementado aca para que cambie cuando tiene que hacerlo
		if (personaje->getVelocity().y <= 0.0f && personaje->getCantidadDeContactosActuales() == 0 && personaje->state != &Personaje::shooting) {
			personaje->state = &Personaje::falling;
			personaje->noAtravezarPlataformas();

		} else if (personaje->getVelocity().y <= 0.0f && personaje->state == &Personaje::jumping) {
			personaje->state = &Personaje::standby;
		}

		if (personaje->movimientoLateralDerecha || personaje->movimientoLateralIzquierda)
			Personaje::walking.caminar(*personaje);

		if (personaje->debeSaltar && personaje->state->getCode() != JUMPING && personaje->state->getCode() != FALLING && personaje->getCantidadDeContactosActuales() !=0) {
			personaje->jump();
			personaje->state = &Personaje::jumping;
		}
	}
	//Seteamos esto aca que me parece lo mas facil, e intuitivo.
	if( Personaje* pers = dynamic_cast< Personaje* >( personaje ) ){
		//Disminuyo el cooldown de patear.
		pers->decreaseKickCooldown();
		if(pers->state == &Personaje::kicking && pers->getKickCooldown() == 0)
			pers->state = &Personaje::standby;
		if(pers->esta_muerto){
			pers->volverAPosicionInicial();
			pers->esta_muerto = false;
			pers->state = &Personaje::standby;
		}
	}
}

void Escenario::clean(){
	for (auto proy = proyectiles_->begin(); proy != proyectiles_->end(); ++proy) {
		b2Body* body = (*proy)->getb2Body();
		for (b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next) {
			b2Contact* c = ce->contact;
			if (c->IsTouching()) {
				world_->DestroyBody((*proy)->getb2Body());
				proyectiles_->erase(proy++);
				break;
			}
		}
	}
}

void Escenario::step() {
	for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
		if (strcmp((*personaje)->id, "sin asignar") != 0)
			acomodarEstadoCharacter(*personaje);
	}
	for (auto enemigo = enemigos_->begin(); enemigo != enemigos_->end(); ++enemigo) {
		if(!(*enemigo)->estaVivo){
			world_->DestroyBody((*enemigo)->getb2Body());
			enemigos_->erase(enemigo++);
		}else
			acomodarEstadoCharacter(*enemigo);
	}
	clean();

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

/*
 * Devolvemos un vector con objetos Estaticos
 */
figura_t* Escenario::getObjetosEstaticos() {

	figura_t* obj;
	Figura* fig;
	obj = (figura_t*) malloc(sizeof(figura_t) * figurasEstaticas_->size());

	for (unsigned int i = 0; i < figurasEstaticas_->size(); i++) {

		fig = (*figurasEstaticas_)[i];

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
 * Devolvemos un vector con objetos dinamicos
 */
figura_t* Escenario::getObjetosDinamicos() {

	figura_t* obj;
	Figura* fig;
	obj = (figura_t*) malloc(sizeof(figura_t) * figurasDinamicas_->size());

	for (unsigned int i = 0; i < figurasDinamicas_->size(); i++) {

		fig = (*figurasDinamicas_)[i];

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
	for (auto proy = proyectiles_->begin(); proy != proyectiles_->end(); ++proy) {

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
	enemigo_t* enems = (enemigo_t*) malloc(sizeof(enemigo_t) * enemigos_->size());

	int i = 0;
	for (auto enemigo = enemigos_->begin(); enemigo != enemigos_->end(); ++enemigo) {
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

void movimientoDeLaBola(Enemigo* enemigo){
	//Empieza con la orientacion actual hasta que toca una pared, donde cambia el sentido.
	//A la segunda pared que toca, se destruye
	if (enemigo->getOrientacion() == 'l')
		enemigo->handleInput(SDLK_LEFT, SDL_KEYDOWN);
	else
		enemigo->handleInput(SDLK_RIGHT, SDL_KEYDOWN);

	//hay dos chances de contacto: piso y pared o piso y personaje
	b2Body* body = enemigo->getb2Body();
	int contactosEstaticos = 0;
	for (b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		Figura *figuraA = (Figura*) c->GetFixtureA()->GetUserData();
		Figura *figuraB = (Figura*) c->GetFixtureB()->GetUserData();

		if ((figuraA->type == ID_ENEMIGO )&& (figuraB->type == ID_PERSONAJE))
			continue;
		else if ((figuraB->type == ID_ENEMIGO) && (figuraA->type == ID_PERSONAJE))
			continue;
		else{
			contactosEstaticos++;
		}
	}
	if(contactosEstaticos < 3)return;
	//Si no salio, quiere decir que esta enContactoConParedYPiso
	//Usamos un cooldown (como en todas las demas cosas que no nos salen) para que cambie la
	//orientacion solo cuando no esta en cooldown.
	//TODO revisar por que siempre tira que hay 2 contactos cuando deberia haber uno. Cuando pega con
	//una pared, hay hasta 5.
	turnCooldown++;
	turnCooldown = turnCooldown%5;
	//Cambiamos la orientacion
	if (turnCooldown != 0) return;
	if (enemigo->getOrientacion() == 'l'){
		enemigo->handleInput(SDLK_LEFT, SDL_KEYUP);
		enemigo->setOrientacion('r');
	}
	else{
		enemigo->handleInput(SDLK_RIGHT, SDL_KEYUP);
		enemigo->setOrientacion('l');
	}

	enemigo->cantidadDeRebotesParaDestruccion++;

	if(enemigo->cantidadDeRebotesParaDestruccion == 3)
		enemigo->estaVivo = false;
}

void Escenario::movimientoDelEnemigo(Enemigo* enemigo){
	int v1 = rand() % 100;
	if(v1 < 45){
		v1 = rand() % 100;
		if (v1 <= 25)
			enemigo->handleInput(SDLK_LEFT, SDL_KEYDOWN);
		else {
			if (v1 <= 50)
				enemigo->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
			else {
				if (v1 <= 75)
					enemigo->handleInput(SDLK_RIGHT, SDL_KEYUP);
				else
					enemigo->handleInput(SDLK_LEFT, SDL_KEYUP);
			}
		}
	}
	else if(v1 < 50){
		   int i=0;
		   float posicionesX[4];
		   float posicionesY[4];
		   for (auto personaje = personajes_->begin(); personaje != personajes_->end(); ++personaje) {
			   posicionesX[i] = (*personaje)->getX();
			   posicionesY[i] = (*personaje)->getY();
			   i++;
			}

			float posicionPersonajeX = posicionesX[0];
			float posicionPersonajeY = posicionesY[0];
			float posicionEnemigoX = enemigo->getX();
			float posicionEnemigoY = enemigo->getY();

			if(posicionPersonajeX < posicionEnemigoX ){
				enemigo->handleInput(SDLK_RIGHT, SDL_KEYUP);
				enemigo->handleInput(SDLK_LEFT, SDL_KEYDOWN);
			}
			if(posicionPersonajeX > posicionEnemigoX){
				enemigo->handleInput(SDLK_LEFT, SDL_KEYUP);
				enemigo->handleInput(SDLK_RIGHT, SDL_KEYDOWN);
			}
			if((posicionPersonajeY-3) > posicionEnemigoY){
				enemigo->handleInput(SDLK_UP, SDL_KEYDOWN);
			}
			if((posicionPersonajeY+1) < posicionEnemigoY)
				enemigo->handleInput(SDLK_UP, SDL_KEYUP);
	}
	else if(v1 > 85)
		enemigo->atravezarPlataformas();
}

void Escenario::actualizarEnemigos() {

	for (auto enemigo = enemigos_->begin(); enemigo != enemigos_->end(); enemigo++) {
		//Analizamos si el enemigo es atravezable
		if((*enemigo)->esAtravezable)
			(*enemigo)->hacerAtravezable();
		else
			(*enemigo)->hacerNoAtravezable();

		if ((*enemigo)->enMovimientoBola){
			movimientoDeLaBola(*enemigo);
		}
		else{
			movimientoDelEnemigo(*enemigo);
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
