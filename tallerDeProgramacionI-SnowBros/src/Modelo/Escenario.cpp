#include "../../headers/Modelo/Escenario.h"

Escenario::Escenario(JsonParser *parser) {
	// Define the gravity vector and then create an instance of b2world
	b2Vec2 gravity(0.0f, -10.0f);
	world_ = new b2World(gravity);

	figuras = parser->getObjetos(); //getObjetos() deberia devolver lista de figuras en lugar de lista de b2Bodies

}

Escenario::~Escenario() {
	delete world_;
	delete figuras;
}

std::list<Figura>* Escenario::getFiguras(){
	return &figuras;
}
