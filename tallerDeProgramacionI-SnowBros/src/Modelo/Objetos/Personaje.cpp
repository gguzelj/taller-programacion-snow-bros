#include "../../../headers/Modelo/Objetos/Personaje.h"

Personaje::Personaje(JsonParser *parser, b2World* world){

	this->stickiness = 0; 	//Lo inicializo en 0
	this->world = world;
	this->velocidadLimite = 15.0f;
	this->aceleracion = 7.4f;
	this->frenado = 0.97;
	this->duracionStickiness = 25;
	contactosSensor = 0;	//Inicio con 0, y cuando empiece tocando algo va a ir subiendo
	tiempoEvitaSaltos = 0;	//Inicio en 0 para que pueda saltar apenas toque una superficie

	world->SetContactListener(&contactos); 	//Logro sobrescribir el b2ContactListener

	//definiendo el body del personaje
	b2BodyDef cuerpoDelPersonaje;
	cuerpoDelPersonaje.type = b2_dynamicBody;                       //El personaje sera un cuerpo dinamico
	cuerpoDelPersonaje.angle = 0;                                   //Angulo 0 asi empieza recto
	cuerpoDelPersonaje.position.Set(0,0);                           //donde queremos que empiece
	this->body = this->world->CreateBody(&cuerpoDelPersonaje);
	this->body->SetFixedRotation(true);		//Evito que rote

	//definiendo ahora el fixture del personaje
	b2PolygonShape formaDelPersonaje;						//esto va a ser si queremos que sea una caja...
	formaDelPersonaje.SetAsBox(1,2);						//...con las siguientes dimensiones
	b2FixtureDef fixtureDelPersonaje;						//creo el fixture
	fixtureDelPersonaje.shape = &formaDelPersonaje;			//le asigno la forma que determine antes
	fixtureDelPersonaje.density = 1;						//una densidad cualquiera
	fixtureDelPersonaje.friction = 0.5;						//Le invento una friccion
	body->CreateFixture(&fixtureDelPersonaje);

	//sensor tiene que ir al final porque meti un sensor
	formaDelPersonaje.SetAsBox(1, 2, b2Vec2(0,-3), 0);	//Le agrego otro fixture al personaje que va a servir como sensor, notar que esta muy relacionado con el tamaño del personaje, si se cambia cambiar esto tambien
	fixtureDelPersonaje.isSensor = true;					//Notar que ahora el fixture nuevo es sensor
	b2Fixture* fixtureDelSensor = body->CreateFixture(&fixtureDelPersonaje);
	fixtureDelSensor->SetUserData( (void*)DE_PIES );		//Le digo que es el pie
}

Personaje::~Personaje(){
	//una posible destruccion del personaje
	this->world->DestroyBody(this->body);
}

void Personaje::Moverse(){
	b2Vec2 velocidadActual = this->body->GetLinearVelocity(); //va a servir para cambiarla
	switch (this->movimientoActual)
	{
		case Mov_Izquierda:
			velocidadActual.x = b2Max( (velocidadActual.x)-(this->aceleracion), (this->velocidadLimite)*-1.0f ); //Le fijo una aceleracion y velocidad maxima
			break;

		case Mov_Derecha:
			velocidadActual.x = b2Min( (velocidadActual.x)+(this->aceleracion), (this->velocidadLimite)*1.0f );  //Le fijo una aceleracion y velocidad maxima
			break;

		case Mov_Nada: //no se apreto boton
			velocidadActual.x = velocidadActual.x * (this->frenado);
			break;
	}
	this->body->SetLinearVelocity( velocidadActual );    //Finalmente es aplicada

    if (velocidadActual.y<=0.01 && velocidadActual.y >= -0.01 && stickiness > this->duracionStickiness){
        stickiness = 0;
        this->movimientoActual = Mov_Nada;
    }
	stickiness++;
	tiempoEvitaSaltos--;
    //ESTO ULTIMO DEBERIA ESTAR ADENTRO DEL STEP
}

void Personaje::Keyboard(unsigned char key)
{
	switch (key)
	{
		case 'a': //TODO Deberia ser cambiado a flecha izquierda
			this->movimientoActual = Mov_Izquierda;
			this->movimientoAnterior = Mov_Izquierda;
			break;

		case 'd': //TODO Deberia ser cambiado a flecha derecha
			this->movimientoActual = Mov_Derecha;
			this->movimientoAnterior = Mov_Derecha;
			break;

		case 'w': //TODO Deberia ser cambiado a flecha arriba
		  if ( tiempoEvitaSaltos > 0 ) break;	//No lo dejo saltar
		  if(contactosSensor>0)   //Voy a ver que este sobre algo para saltar
			this->Saltar(this->body->GetLinearVelocity());
			break;
		default: //stop
			this->movimientoActual = Mov_Nada;
			break;
	}
}

void Personaje::Saltar(b2Vec2 velocidadActual){
	 float potenciaDeSalto = this->body->GetMass() * 7;
	 this->body->ApplyLinearImpulse( b2Vec2(0,potenciaDeSalto), this->body->GetWorldCenter(),true ); //Se podria cambiar el this->body->GetWorldCenter() por this->body->GetLocalCenter()
	 this->movimientoActual = this->movimientoAnterior;    //Para que el salto no interrumpa el movimiento que tenia de antes
	 tiempoEvitaSaltos = 15;
}


b2Fixture* Personaje::GetFixtureList(){
	return this->body->GetFixtureList();
}


b2Vec2 Personaje::GetWorldPoint(const b2Vec2& localPoint){
	return body->GetWorldPoint(localPoint);
}
