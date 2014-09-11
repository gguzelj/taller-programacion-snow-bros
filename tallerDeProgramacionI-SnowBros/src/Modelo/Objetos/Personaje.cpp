#include "../../../headers/Modelo/Objetos/Personaje.h"

Personaje::Personaje(JsonParser *parser, b2World* world){

        this->stickiness = 0; //Lo inicializo en 0
        this->world = world;
        this->velocidadLimite = 15.0f;
        this->aceleracion = 7.4f;
        this->frenado = 0.97;
        this->duracionStickiness = 25;

        //definiendo el body del personaje
        b2BodyDef cuerpoDelPersonaje;
        cuerpoDelPersonaje.type = b2_dynamicBody;                       //El personaje sera un cuerpo dinamico
        cuerpoDelPersonaje.angle = 0;                                           //Angulo 0 asi empieza recto
        cuerpoDelPersonaje.position.Set(0,0);                           //donde queremos que empiece

        b2Body* cuerpoDinamicoDelPersonaje = this->world->CreateBody(&cuerpoDelPersonaje);
        this->cuerpoDinamicoPersonaje = cuerpoDinamicoDelPersonaje;

        //definiendo ahora el fixture del personaje
        b2PolygonShape formaDelPersonaje;                                       //esto va a ser si queremos que sea una caja...
        formaDelPersonaje.SetAsBox(1,1);                                        //...con las siguientes dimensiones
        b2FixtureDef fixtureDelPersonaje;                                       //creo el fixture
        fixtureDelPersonaje.shape = &formaDelPersonaje;         //le asigno la forma que determine antes
        fixtureDelPersonaje.density = 1;                                        //una densidad cualquiera
        cuerpoDinamicoDelPersonaje->CreateFixture(&fixtureDelPersonaje);

}
Personaje::~Personaje(){
        //una posible destruccion del personaje
//        this->world->DestroyBody(this->cuerpoDinamicoPersonaje); TIRA ERROR
        //no se si me estare olvidando de algun destructor
}

void Personaje::Moverse(){
        b2Vec2 velocidadActual = this->cuerpoDinamicoPersonaje->GetLinearVelocity(); //va a servir para cambiarla
        switch (this->movimientoActual)
                {
                case Mov_Izquierda:
                        velocidadActual.x = b2Max( (velocidadActual.x)-(this->aceleracion), (this->velocidadLimite)*-1.0f ); //Le fijo una aceleracion y velocidad maxima
                break;

                case Mov_Derecha:
                    velocidadActual.x = b2Min( (velocidadActual.x)+(this->aceleracion), (this->velocidadLimite)*1.0f ); //Le fijo una aceleracion y velocidad maxima
                        break;

                case Mov_Nada: //no se apreto boton
                        velocidadActual.x = velocidadActual.x * (this->frenado);
                        break;
                }
        this->cuerpoDinamicoPersonaje->SetLinearVelocity( velocidadActual );    //Finalmente es aplicada

    if (velocidadActual.y<=0.01 && velocidadActual.y >= -0.01 && stickiness > this->duracionStickiness){
        stickiness = 0;
        this->movimientoActual = Mov_Nada;
    }
        stickiness++;
    //ESTO ULTIMO DEBERIA ESTAR ADENTRO DEL STEP
}

void Personaje::Keyboard(unsigned char key)
{
        switch (key)
        {
          case 'a': //Deberia ser cambiado a flecha izquierda
                this->movimientoActual = Mov_Izquierda;
                this->movimientoAnterior = Mov_Izquierda;
                break;

          case 'd': //Deberia ser cambiado a flecha derecha
                this->movimientoActual = Mov_Derecha;
                this->movimientoAnterior = Mov_Derecha;
                break;

          case 'w': //Deberia ser cambiado a flecha arriba
                this->Saltar(this->cuerpoDinamicoPersonaje->GetLinearVelocity());
                        break;
          default: //stop
                  this->movimientoActual = Mov_Nada;
                break;
        }
}

void Personaje::Saltar(b2Vec2 velocidadActual){
          if(velocidadActual.y < 0.01f && velocidadActual.y > -0.01f)   //Voy a ver que este quieto para saltar, si uso velocidad.y==0 tengo problemas
          {
                float potenciaDeSalto = this->cuerpoDinamicoPersonaje->GetMass() * 7;
                this->cuerpoDinamicoPersonaje->ApplyLinearImpulse( b2Vec2(0,potenciaDeSalto), this->cuerpoDinamicoPersonaje->GetWorldCenter(),true );
          };
          this->movimientoActual = this->movimientoAnterior;    //Para que el salto no interrumpa el movimiento que tenia de antes
}


