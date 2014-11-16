#include "../../../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../../../headers/Modelo/Escenario.h"

#define ORIENTACION_INICIAL 'l'

Personaje::Personaje(float x, float y, conn_id id, Escenario* escenario) {

	//Parametros generales
	this->escenario_ = escenario;
	this->world = escenario->getWorld();
	this->jumpCooldown = 0;
	this->shootCooldown = 0;
	this->kickCooldown = 0;
	this->aceleracion = 10.0f;
	this->x = x;
	this->y = y;
	this->posicionInicial = new b2Vec2(x, y);
	strcpy(this->id, id);
	this->state = &Personaje::standby;
	this->orientacion = ORIENTACION_INICIAL;
	this->esta_muerto = false;
	this->arrastradoPor = nullptr;
	this->joint = nullptr;

	this->connectionState = CONECTADO;
	this->points = 0;
	this->lives = 0;
	this->type = ID_PERSONAJE;
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	this->movimientoDisparar = false;
	this->movimientoDerecha = false;
	this->movimientoIzquierda = false;
	this->inmune = false;

	//Defino body del personaje
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angle = 0;
	bodyDef.position.Set(x, y);
	bodyDef.gravityScale = 4;
	this->body = this->world->CreateBody(&bodyDef);

	//Defino el Shape del personaje
	b2PolygonShape shapeDelPersonaje;
	shapeDelPersonaje.SetAsBox(ancho, alto);

	//Definimos el fixture del personaje
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDelPersonaje;
	fixtureDef.density = 10;
	fixtureDef.restitution = 0;
	fixtureDef.friction = 0.001f;
	fixtureDef.filter.groupIndex = PERSONAJE_FILTER_INDEX;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelPersonaje.SetAsBox(ancho / 100, alto * 0.002, b2Vec2(-ancho, 0), 0);
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelPersonaje.SetAsBox(ancho / 100, alto * 0.002, b2Vec2(ancho, 0), 0);
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelPersonaje.SetAsBox(ancho * 0.001, alto / 100, b2Vec2(0, -alto), 0);
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

	//Entramos en el periodo de inmunidad
	entrarEnPeriodoDeInmunidad();

}

Personaje::~Personaje() {
	this->world->DestroyBody(this->body);
}

//////////////////////////////////////////////////////////
///              Funciones auxiliares                  ///
//////////////////////////////////////////////////////////

void crearJoint(Personaje* personaje, BolaEnemigo* bolaEnemigo, b2World* world_) {
	b2RevoluteJointDef revjoint;
	revjoint.bodyA = bolaEnemigo->getb2Body();
	revjoint.bodyB = personaje->getb2Body();
	revjoint.collideConnected = false;
	revjoint.localAnchorA.Set(0, 0);
	revjoint.localAnchorB.Set(0, 0);
	personaje->setJoint((b2RevoluteJoint*) world_->CreateJoint(&revjoint));
}

//////////////////////////////////////////////////////////
///              Public methods                        ///
//////////////////////////////////////////////////////////

void Personaje::handleInput(SDL_Keycode input, Uint32 input_type) {

	if (state == &Personaje::dying) {
		state->handleInput(*this, input, input_type);
		return;
	}
	state->handleInput(*this, input, input_type);
}

void Personaje::beginContactBolaEnemigo(BolaEnemigo * bola, b2Contact* contact) {

	if (this->state == &Personaje::jumping)
		return;

	bola->cambiarFilterIndex(PERSONAJE_FILTER_INDEX);

	this->movimientoDerecha = false;
	this->movimientoIzquierda = false;
	this->arrastrado = true;
	this->arrastradoPor = bola;
	this->state = &Personaje::rolling;
	return;
}

void Personaje::beginContactEnemigo(Enemigo* enemigo, b2Contact* contact) {

	if (state == &Personaje::dying || state == &Personaje::rolling)
		return;

	//En otro caso, restamos vida
	if (lives > 0 && !inmune) {
		state = &Personaje::dying;
		sacarVida();
		std::thread t(&Personaje::morir, this);
		t.detach();
	}
	// TODO En caso que el personaje pierda todas sus vidas, el mismo no debe aparecer mas en la pantalla. Es decir,
	// hay que sacarlo del modelo.
}

bool Personaje::estaEmpujandoEnemigo() {
	Figura *figura;
	b2Fixture *pared = (orientacion == DERECHA) ? paredDerecha : paredIzquierda;

	//Buscamos si el piso esta haciendo contacto con algo
	for (b2ContactEdge *ce = this->body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;

		if (c->GetFixtureA() == pared) {
			figura = (Figura*) c->GetFixtureB()->GetUserData();
			if (figura->type == ID_ENEMIGO) {
				return ((Enemigo*) figura)->estaCongelado();
			}
		}

		if (c->GetFixtureB() == pared) {
			figura = (Figura*) c->GetFixtureA()->GetUserData();
			if (figura->type == ID_ENEMIGO) {
				return ((Enemigo*) figura)->estaCongelado();
			}
		}
	}

	return false;
}

void Personaje::controlarEstado() {

	Character::controlarEstado();

	if (state == &Character::walking)
		if (estaEmpujandoEnemigo())
			state = &Character::pushing;

	//Seteamos esto aca que me parece lo mas facil, e intuitivo.
	//Disminuyo el cooldown de patear.
	decreaseKickCooldown();

	if (state == &Personaje::kicking && getKickCooldown() == 0) {
		stop();
		state = &Personaje::standby;
		return;
	}

	if (esta_muerto) {
		volverAPosicionInicial();
		esta_muerto = false;
		state = &Personaje::standby;
		return;
	}

	if (arrastradoPor && !arrastrado) {
		world->DestroyJoint(joint);
		arrastrado = false;
		arrastradoPor = nullptr;
		joint = nullptr;

		b2Transform tra = body->GetTransform();
		tra.p.y += 3;
		body->SetTransform(tra.p, 0);

		state = &Character::jumping;
		jump();
		return;
	}

	if (state == &Personaje::rolling && !joint)
		crearJoint(this, arrastradoPor, world);
}

void Personaje::shoot() {

	if (!movimientoDisparar)
		return;

	if (shootCooldown > 0)
		return;

	shootCooldown = 10;

	BolaNieve *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaNieve(getX() - 1, getY() + MITAD_ALTO_PERSONAJE, 1, this->world);
	else
		bola = new BolaNieve(getX() + 1, getY() + MITAD_ALTO_PERSONAJE, 1, this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	if (orientacion == IZQUIERDA)
		vel.x -= aceleracion * 4;
	else
		vel.x += aceleracion * 4;

	vel.y = 2;

	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

}

void Personaje::kick() {
	kickCooldown = 12;
	//Iteramos con los contactos de nuestro personaje hasta encontrar al enemigo y luego lo matamos
	for (b2ContactEdge *ce = this->body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		Figura *figuraA = (Figura*) c->GetFixtureA()->GetUserData();
		if (figuraA->type == ID_ENEMIGO) {
			((Enemigo*) figuraA)->setOrientacion(this->orientacion);
			((Enemigo*) figuraA)->morir();
			return;
		}
	}
}

void Personaje::jump() {

	if (estaEnAire())
		return;

	if (this->jumpCooldown <= 0) {
		this->jumpCooldown = 18;
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.y = 25;
		this->body->SetLinearVelocity(velocidadActual);
		atravezarPlataformas();
	}
}

void Personaje::morir() {
	sleep(1);
	entrarEnPeriodoDeInmunidad();
	this->esta_muerto = true;
	this->points /= 2;
}

void Personaje::volverAPosicionInicial() {
	this->body->SetTransform(*posicionInicial, body->GetAngle());
}

void Personaje::noAtravezarPlataformas() {
	cambiarFilterIndex(PERSONAJE_FILTER_INDEX);
}

void Personaje::decreaseKickCooldown() {
	if (this->kickCooldown > 0)
		this->kickCooldown -= 1;
}

void Personaje::sacarVida() {
	lives--;
}

void Personaje::setConnectionState(char state) {
	connectionState = state;
}

void Personaje::setJoint(b2RevoluteJoint* joint) {
	this->joint = joint;
}

void Personaje::setArrastrado(bool valor) {
	arrastrado = valor;
}

void Personaje::setArrastradoPor(BolaEnemigo* bola) {
	arrastradoPor = bola;
}

char Personaje::getId() {
	return ID_PERSONAJE;
}

b2Body* Personaje::getb2Body() {
	return body;
}

int Personaje::getPoints() {
	return points;
}

int Personaje::getLives() {
	return lives;
}

bool Personaje::getInmune() {
	return inmune;
}

int Personaje::getKickCooldown() {
	return kickCooldown;
}

b2Joint* Personaje::getJoint() {
	return joint;
}

BolaEnemigo* Personaje::getArrastradoPor() {
	return arrastradoPor;
}

bool Personaje::esArrastrado() {
	return arrastrado;
}

bool Personaje::estaMuerto() {
	return esta_muerto;
}

char Personaje::getConnectionState() {
	return connectionState;
}

//////////////////////////////////////////////////////////
///              Private methods                       ///
//////////////////////////////////////////////////////////

void Personaje::entrarEnPeriodoDeInmunidad() {
	inmune = true;
	std::thread t(&Personaje::hacerInmune, this);
	t.detach();
}

void Personaje::hacerInmune() {
	sleep(TIEMPO_INMUNIDAD);
	inmune = false;
}

void Personaje::addPoints(int puntos){
	points+=puntos;
}
