#include "../../../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../../../headers/Modelo/Escenario.h"

#define ORIENTACION_INICIAL 'l'

Personaje::Personaje(float x, float y, conn_id id, Escenario* escenario, char color) {

	//Parametros generales
	this->escenario_ = escenario;
	this->world = escenario->getWorld();
	this->aceleracion = 10.0f;
	this->x = x;
	this->y = y;
	this->posicionInicial = new b2Vec2(x, y);
	strcpy(this->id, id);
	this->state = &Personaje::standby;
	this->orientacion = ORIENTACION_INICIAL;

	this->jumpCooldown = 0;
	this->shootCooldown = 0;
	this->kickCooldown = 0;
	this->portalCooldown = 0;

	this->color = color;

	this->arrastradoPor = nullptr;
	this->joint = nullptr;
	this->portal1 = nullptr;
	this->portal2 = nullptr;

	this->connectionState = CONECTADO;
	this->potencia = 1;
	this->points = 0;
	this->lives = 3;
	this->type = ID_PERSONAJE;
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	this->esta_muerto = false;
	this->arma_portal = true;
	this->disparar_portal = false;
	this->movimientoDisparar = false;
	this->movimientoDerecha = false;
	this->movimientoIzquierda = false;
	this->teletransportar = false;
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
	shapeDelPersonaje.SetAsBox(ancho / 1000, alto * 0.002, b2Vec2(-ancho, 0), 0);
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelPersonaje.SetAsBox(ancho / 1000, alto * 0.002, b2Vec2(ancho, 0), 0);
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	fixtureDef.friction = 0.7f;
	shapeDelPersonaje.SetAsBox(ancho * 0.001, alto / 1000, b2Vec2(0, -alto), 0);
	piso = this->body->CreateFixture(&fixtureDef);

	//Seteamos esta clase como UserData
	paredIzquierda->SetUserData(this);
	paredDerecha->SetUserData(this);
	piso->SetUserData(this);

	//Entramos en el periodo de inmunidad
	entrarEnPeriodoDeInmunidad();
}

Personaje::~Personaje() {
	world->DestroyBody(this->body);
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
		dejarDisparar();
		detener(IZQUIERDA);
		detener(DERECHA);
		return;
	}
	state->handleInput(*this, input, input_type);
}

void Personaje::beginContactBolaEnemigo(BolaEnemigo * bola, b2Contact* contact) {

	if (state == &Personaje::jumping)
		return;

	bola->cambiarFilterIndex(PERSONAJE_FILTER_INDEX);

	movimientoDerecha = false;
	movimientoIzquierda = false;
	arrastrado = true;
	arrastradoPor = bola;
	state = &Personaje::rolling;
	return;
}

void Personaje::beginContactBolaFuego(BolaFuego* bola, b2Contact* contacto) {

	bola->destruir = true;

	if (state == &Personaje::dying || state == &Personaje::rolling || state == &Personaje::flying)
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

void Personaje::beginContactEnemigo(Enemigo* enemigo, b2Contact* contact) {

	if (state == &Personaje::dying || state == &Personaje::rolling || state == &Personaje::flying)
		return;

	if (enemigo->estaCongelado())
		return;

	//En otro caso, restamos vida
	if (lives > 0 && !inmune) {
		state = &Personaje::dying;
		sacarVida();
		std::thread t(&Personaje::morir, this);
		t.detach();
	}
}

void Personaje::beginContactBonusVidaExtra(BonusVidaExtra* bonus, b2Contact* contact) {
	escenario_->agregarSonido(ONEUP);
	bonus->desactivar();
	lives++;
}

void Personaje::beginContactBonusMoverRapido(BonusMoverRapido* bonus, b2Contact* contact) {
	bonus->desactivar();
	escenario_->agregarSonido(BONUS);
	std::thread t(&Personaje::aumentarVelocidad, this);
	t.detach();
}

void Personaje::beginContactBonusAumentarPotencia(BonusAumentarPotencia* bonus, b2Contact* contact) {
	escenario_->agregarSonido(BONUS);
	bonus->desactivar();
	potencia +=4;
}

void Personaje::beginContactBonusBolaPortal(BonusBolaPortal* bonus, b2Contact* contact) {
	escenario_->agregarSonido(BONUS);
	bonus->desactivar();
	arma_portal = true;
	portal1 = nullptr;
	portal2 = nullptr;
}

bool Personaje::estaEmpujandoEnemigo() {
	Figura *figura;
	b2Fixture *pared = (orientacion == DERECHA) ? paredDerecha : paredIzquierda;

	//Buscamos si el piso esta haciendo contacto con algo
	for (b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;

		if (c->GetFixtureA() == pared)
			figura = (Figura*) c->GetFixtureB()->GetUserData();
		else if (c->GetFixtureB() == pared)
			figura = (Figura*) c->GetFixtureA()->GetUserData();
		else
			continue;

		if (figura->type == ID_ENEMIGO_BASICO || figura->type == ID_ENEMIGO_FUEGO)
			return ((Enemigo*) figura)->estaCongelado();

	}

	return false;
}

void Personaje::controlarEstado() {

	Character::controlarEstado();

	if(state == &Character::flying){
		this->movimientoDerecha = false;
		this->movimientoIzquierda = false;
	}

	decreaseShootPortalCooldown();
	shootPortal();

	if (state == &Character::walking)
		if (estaEmpujandoEnemigo())
			state = &Character::pushing;

	//Si ya disparamos los dos portales, cambiamos el arma
	if (portal1 && portal2)
		arma_portal = false;

//Seteamos esto aca que me parece lo mas facil, e intuitivo.
//Disminuyo el cooldown de patear.
	decreaseKickCooldown();

	if (state == &Personaje::kicking && getKickCooldown() == 0) {
		stop(movimientoIzquierda,movimientoDerecha);
		state = &Personaje::standby;
		return;
	}

	if (esta_muerto) {
		stop(movimientoIzquierda,movimientoDerecha);
		dejarDisparar();
		dejarDispararPortal();
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
		tra.p.y += 1;
		body->SetTransform(tra.p, 0);

		state = &Character::jumping;
		jump();
		return;
	}

	if (state == &Personaje::rolling && !joint)
		crearJoint(this, arrastradoPor, world);
}

Proyectil* Personaje::crearBolaPortal() {

	BolaPortal *bola;
	b2Vec2 vel;

	float x = getX();
	float y = getY() + MITAD_ALTO_PERSONAJE;

	x += (orientacion == IZQUIERDA) ? -1.5 : 1.5;

	bola = new BolaPortal(x, y, 1, world, this);

	vel = body->GetLinearVelocity();
	vel.x -= (orientacion == IZQUIERDA) ? aceleracion * 4 : -aceleracion * 4;
	vel.y = 0;

	bola->setVelocidad(vel);

	return (Proyectil*) bola;
}
Proyectil* Personaje::crearBolaNieve() {

	BolaNieve *bola;
	b2Vec2 vel;

	float x = getX();
	float y = getY() + MITAD_ALTO_PERSONAJE;

	x += (orientacion == IZQUIERDA) ? -1.5 : 1.5;

	bola = new BolaNieve(x, y, potencia, world);

	vel = body->GetLinearVelocity();
	vel.x -= (orientacion == IZQUIERDA) ? aceleracion * 4 : -aceleracion * 4;
	vel.y = 2;

	bola->setVelocidad(vel);

	return (Proyectil*) bola;
}


void Personaje::shoot() {

	if (!movimientoDisparar || shootCooldown > 0)
		return;

	shootCooldown = SHOOTCOOLDOWN;

	escenario_->agregarProyectil(crearBolaNieve());
}

void Personaje::shootPortal(){
	if(!disparar_portal || portalCooldown > 0)
		return;

	portalCooldown = PORTALCOOLDOWN;

	if (arma_portal)
		escenario_->agregarProyectil(crearBolaPortal());
}

void Personaje::kick() {
	kickCooldown = KICKCOOLDOWN;
	//Iteramos con los contactos de nuestro personaje hasta encontrar al enemigo y luego lo matamos
	for (b2ContactEdge *ce = body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		Figura *figuraA = (Figura*) c->GetFixtureA()->GetUserData();
		if (figuraA->type == ID_ENEMIGO_BASICO || figuraA->type == ID_ENEMIGO_FUEGO) {
			((Enemigo*) figuraA)->setOrientacion(orientacion);
			((Enemigo*) figuraA)->morir();
			return;
		}
	}
}

void Personaje::jump() {

	if (estaEnAire())
		return;

	if (jumpCooldown <= 0) {
		jumpCooldown = JUMPCOOLDOWN;
		b2Vec2 velocidadActual = body->GetLinearVelocity();
		velocidadActual.y = 25;
		body->SetLinearVelocity(velocidadActual);
		atravezarPlataformas();
	}
}

void Personaje::dispararPortal(){
	disparar_portal = true;
}

void Personaje::dejarDispararPortal(){
	disparar_portal = false;
}

void Personaje::morir() {
	if(lives > 0)
		escenario_->agregarSonido(DYING);
	else
		escenario_->agregarSonido(GAMEOVER);
	sleep(1);
	entrarEnPeriodoDeInmunidad();
	esta_muerto = true;
	points /= 2;
}

void Personaje::aumentarVelocidad() {
	aceleracion += 5;
	sleep(15);
	aceleracion -= 5;
}

void Personaje::volverAPosicionInicial() {
	body->SetTransform(*posicionInicial, body->GetAngle());
}

void Personaje::noAtravezarPlataformas() {
	cambiarFilterIndex(PERSONAJE_FILTER_INDEX);
}

void Personaje::decreaseKickCooldown() {
	if (kickCooldown > 0)
		kickCooldown--;
}

void Personaje::decreaseShootPortalCooldown() {
	if(portalCooldown > 0)
		portalCooldown--;
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

int Personaje::getJumpCooldown(){
	return jumpCooldown;
}

int Personaje::getShootCooldown(){
	return shootCooldown;
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

void Personaje::addPoints(int puntos) {
	points += puntos;
}
