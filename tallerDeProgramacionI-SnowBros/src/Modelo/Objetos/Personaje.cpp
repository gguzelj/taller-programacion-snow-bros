#include "../../../headers/Modelo/Objetos/Personaje.h"

Personaje::Personaje(JsonParser *parser, b2World* world){

	this->world = new b2World(b2Vec2(0, -9.8f)); //Nota: esto no se crea aca, se pasa por parametro. Lo dejo por ahora para que no tire warning

	this->movimientoIzquierda = -1;
	this->movimientoDerecha = 1;
	this->movimientoNada = 0;
	this->velocidadLimite = 20;
	this->aceleracion = 10;
	this->frenado = -10;
	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody; 			//El personaje sera un cuerpo dinamico
	cuerpoDelPersonaje.angle = 0; 						//Angulo 0 asi empieza recto
	cuerpoDelPersonaje.position.Set(0,0); 				//donde queremos que empiece

	b2Body* cuerpoDinamicoDelPersonaje = this->world->CreateBody(&cuerpoDelPersonaje);
	this->cuerpoDinamicoPersonaje = cuerpoDinamicoDelPersonaje;

	//definiendo ahora el fixture del personaje
	b2PolygonShape formaDelPersonaje;					//esto va a ser si queremos que sea una caja...
	formaDelPersonaje.SetAsBox(1,1); 					//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;					//creo el fixture
	fixtureDelPersonaje.shape = &formaDelPersonaje;		//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;					//una densidad cualquiera
	cuerpoDinamicoDelPersonaje->CreateFixture(&fixtureDelPersonaje);

}
Personaje::~Personaje(){
	//una posible destruccion del personaje
	this->world->DestroyBody(this->cuerpoDinamicoPersonaje);
	//no se si me estare olvidando de algun destructor
}

void Personaje::Moverse(int numeroIngresado){
	//Movimiento con aceleracion, podriamos hacer que si deja de tocar una tecla se empiece a frenar bruscamente
	float apliqueDeEmpuje;
	b2Vec2 velocidadActual = this->cuerpoDinamicoPersonaje->GetLinearVelocity(); //va a servir para cambiarla
	switch (numeroIngresado)
	{
	case -1: //movimiento a izquierda
		if (velocidadActual.x > -(this->velocidadLimite))
		apliqueDeEmpuje = -(this->aceleracion);
	break;

	case 1:	//movimiento a derecha
		if (velocidadActual.x < this->velocidadLimite)
			apliqueDeEmpuje = this->aceleracion;
		break;

	case 0: //no se apreto boton
		apliqueDeEmpuje = velocidadActual.x * (this->frenado);
		break;
	}
	this->cuerpoDinamicoPersonaje->ApplyForce(b2Vec2(apliqueDeEmpuje,0), this->cuerpoDinamicoPersonaje->GetWorldCenter(),true); //tuve que agregar el true porque cambiaron el applyforce, no se que pasa si dejo false
}

void Personaje::Saltar(){

}
