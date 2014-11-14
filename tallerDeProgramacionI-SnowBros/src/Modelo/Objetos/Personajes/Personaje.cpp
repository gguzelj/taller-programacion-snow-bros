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
	this->puedeEmpujar = false;
	this->arrastradoPor = nullptr;
	this->joint = nullptr;

	this->connectionState = CONECTADO;
	this->points = 0;
	this->lives = 0;
	this->type = ID_PERSONAJE;
	this->ancho = MITAD_ANCHO_PERSONAJE;
	this->alto = MITAD_ALTO_PERSONAJE;

	this->movimientoLateralDerecha = false;
	this->movimientoLateralIzquierda = false;
	this->debeSaltar = false;
	this->inmune = false;

	//Parametros para controlar los contactos
	this->contactosActuales = 0;
	this->contactosIzquierda = 0;
	this->contactosDerecha = 0;

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
	fixtureDef.filter.groupIndex = PERSONAJE_FILTER_INDEX;
	b2Fixture* fix = body->CreateFixture(&fixtureDef);

	//Actualizamos informacion adicional
	this->body->SetFixedRotation(true);
	this->body->SetLinearDamping(0.5);

	fix->SetUserData(this);

	//Pared Izquierda
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f,
			b2Vec2(-ancho + 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredIzquierda = this->body->CreateFixture(&fixtureDef);

	//ParedDerecha
	shapeDelPersonaje.SetAsBox(0.0000001f, alto - 0.00405f,
			b2Vec2(ancho - 0.00000005, 0.0045f), 0);
	fixtureDef.friction = 0.0019f;
	paredDerecha = this->body->CreateFixture(&fixtureDef);

	//Piso
	shapeDelPersonaje.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto),
			0);
	fixtureDef.friction = 0.0019f;
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

void Personaje::disparar() {

	if (shootCooldown > 0)
		return;

	shootCooldown = 10;

	BolaNieve *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaNieve(getX() - 1, getY() + MITAD_ALTO_PERSONAJE, 1,
				this->world);
	else
		bola = new BolaNieve(getX() + 1, getY() + MITAD_ALTO_PERSONAJE, 1,
				this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	if (orientacion == IZQUIERDA)
		vel.x -= aceleracion * 15;
	else
		vel.x += aceleracion * 15;

	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

}

void Personaje::empujar() {
	kickCooldown = 12;
	//Iteramos con los contactos de nuestro personaje hasta encontrar al enemigo y luego lo matamos
	for (b2ContactEdge *ce = this->body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		Figura *figuraA = (Figura*) c->GetFixtureA()->GetUserData();
		if (figuraA->type == ID_ENEMIGO) {
			points += ((Enemigo*) figuraA)->getPuntos();
			((Enemigo*) figuraA)->setOrientacion(this->orientacion);
			((Enemigo*) figuraA)->morir();
			return;
		}
	}
}

void Personaje::handleInput(SDL_Keycode input, Uint32 input_type) {

	if (state == &Personaje::dying) {
		state->handleInput(*this, input, input_type);
		return;
	}
	state->handleInput(*this, input, input_type);
}

void Personaje::beginContactBolaEnemigo(BolaEnemigo * bola,
		b2Contact* contact) {

	if (this->state == &Personaje::jumping)
		return;

	bola->cambiarFilterIndex(PERSONAJE_FILTER_INDEX);

	this->arrastrado = true;
	this->arrastradoPor = bola;
	this->state = &Personaje::rolling;
	return;

}

/*
 * Aca definimos como reacciona el personaje ante el contacto con el enemigo.
 * El enemigo se pasa por parametro para que se pueda definir su comportamiento tambien
 */
void Personaje::beginContactEnemigo(Enemigo* enemigo, b2Contact* contact) {

	if (state == &Personaje::dying || state == &Personaje::rolling)
		return;

	//Si el enemigo esta congelado, no nos sucede nada
	if (enemigo->estaCongelado()) {
		b2Fixture* fixture1;
		b2Fixture* fixture2;
		if (this->getOrientacion() == 'l') {
			fixture1 = this->paredIzquierda;
			fixture2 = enemigo->paredDerecha;
			enemigo->setOrientacion('r');
		} else {
			fixture1 = this->paredDerecha;
			fixture2 = enemigo->paredIzquierda;
			enemigo->setOrientacion('l');
		}
		//Checkeo si lo que se esta overlappeando son las paredes de los costados. Si no lo son, entonces es porque no debe
		//estar empujando.
		if (b2TestOverlap(fixture1->GetShape(), 0, fixture2->GetShape(), 0,
				body->GetTransform(), enemigo->getb2Body()->GetTransform())) {
			state = &Personaje::pushing;
		}
		return;
	}

	//En otro caso, restamos vida
	if (lives > 0 && !inmune) {
		state = &Personaje::dying;
		sacarVida();
		std::thread t(&Personaje::morir, this);
		t.detach();
	}
	// En caso que el personaje pierda todas sus vidas, el mismo no debe aparecer mas en la pantalla. Es decir,
	// hay que sacarlo del modelo. TODO
}

void Personaje::morir() {
	sleep(1);
	entrarEnPeriodoDeInmunidad();
	this->esta_muerto = true;
	this->points /= 2;
}

void Personaje::jump() {
	if (this->jumpCooldown <= 0) {
		this->jumpCooldown = 18;
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.y = 25;
		this->body->SetLinearVelocity(velocidadActual);
		atravezarPlataformas();
	}
}

void Personaje::decreaseKickCooldown() {
	if (this->kickCooldown > 0)
		this->kickCooldown -= 1;
}

void Personaje::volverAPosicionInicial() {
	this->body->SetTransform(*posicionInicial, body->GetAngle());
}

void Personaje::entrarEnPeriodoDeInmunidad() {

	inmune = true;

	std::thread t(&Personaje::hacerInmune, this);
	t.detach();

}

void Personaje::hacerInmune() {

	sleep(TIEMPO_INMUNIDAD);

	inmune = false;
}

void Personaje::noAtravezarPlataformas() {
	cambiarFilterIndex(PERSONAJE_FILTER_INDEX);
}

char Personaje::getId() {
	return ID_PERSONAJE;
}

void crearJoint(Personaje* personaje, BolaEnemigo* bolaEnemigo,
		b2World* world_) {
	b2RevoluteJointDef revjoint;
	revjoint.bodyA = bolaEnemigo->getb2Body();
	revjoint.bodyB = personaje->getb2Body();
	revjoint.collideConnected = false;
	revjoint.localAnchorA.Set(0, 0);
	revjoint.localAnchorB.Set(0, 0);
	personaje->joint = (b2RevoluteJoint*) world_->CreateJoint(&revjoint);
}

void Personaje::controlarEstado() {
	Character::controlarEstado();

	//Seteamos esto aca que me parece lo mas facil, e intuitivo.
	//Disminuyo el cooldown de patear.
	decreaseKickCooldown();
	if (state == &Personaje::kicking && getKickCooldown() == 0){
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
		debeSaltar = true;

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

void Personaje::sacarVida() {
	lives--;
}

char Personaje::getConnectionState() {
	return connectionState;
}
void Personaje::setConnectionState(char state) {
	connectionState = state;
}
