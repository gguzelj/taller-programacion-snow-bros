#include "../../../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../../../headers/Modelo/Escenario.h"

#define ORIENTACION_INICIAL 'l'

Personaje::Personaje(float x, float y, conn_id id, Escenario* escenario) {

	//Parametros generales
	this->escenario_ = escenario;
	this->world = escenario->getWorld();
	this->jumpCooldown = 0;
	this->shootCooldown = 0;
	this->aceleracion = 10.0f;
	this->x = x;
	this->y = y;
	this->posicionInicial = new b2Vec2(x, y);
	strcpy(this->id, id);
	this->state = &Personaje::standby;
	this->orientacion = ORIENTACION_INICIAL;
	this->esta_muerto = false;
	this->puedeEmpujar = false;

	this->connectionState = CONECTADO;
	this->points = 0;
	this->lives = 5;
	this->type = "personaje";
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
	fixtureDef.density = 1;
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
	shapeDelPersonaje.SetAsBox(ancho * 19.5f / 20, alto / 10, b2Vec2(0, -alto), 0);
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

	if(shootCooldown > 0) return;

	shootCooldown = 10;

	BolaNieve *bola;

	if (orientacion == IZQUIERDA)
		bola = new BolaNieve(getX() - 1, getY() + MITAD_ALTO_PERSONAJE - 0.40,
				1, this->world);
	else
		bola = new BolaNieve(getX() + 1, getY() + MITAD_ALTO_PERSONAJE - 0.40,
				1, this->world);

	b2Vec2 vel = this->body->GetLinearVelocity();

	if (orientacion == IZQUIERDA)
		vel.x -= aceleracion * 2.0;
	else
		vel.x += aceleracion * 2.0;
	vel.y += 10;

	bola->setVelocidad(vel);

	this->escenario_->agregarProyectil(bola);

}

void Personaje::empujar() {

	//Iteramos con los contactos de nuestro personaje hasta encontrar al enemigo y luego lo matamos
	for (b2ContactEdge *ce = this->body->GetContactList(); ce; ce = ce->next) {
		b2Contact* c = ce->contact;
		Figura *figuraA = (Figura*) c->GetFixtureA()->GetUserData();
		if (figuraA->type == "enemigo") {
			((Enemigo*) figuraA)->morir();
			points++;
			return;
		}
	}
}

void Personaje::handleInput(SDL_Keycode input, Uint32 input_type) {
	if(state == &Personaje::dying) return;
	state->handleInput(*this, input, input_type);
}

/*
 * Aca definimos como reacciona el personaje ante el contacto con el
 * enemigo.
 * El enemigo se pasa por parametro para que se pueda definir su comportamiento tambien
 */
void Personaje::reaccionarConEnemigo(Enemigo* enemigo) {

	if(state == &Personaje::dying) return;

	//Si el enemigo esta congelado, no nos sucede nada
	if (enemigo->estaCongelado()) {
		state = &Personaje::pushing;
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
}

void Personaje::jump() {
	if (this->jumpCooldown <= 0) {
		this->jumpCooldown = 18;
		b2Vec2 velocidadActual = this->body->GetLinearVelocity();
		velocidadActual.y = 25;
		this->body->SetLinearVelocity(velocidadActual);
	}
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
