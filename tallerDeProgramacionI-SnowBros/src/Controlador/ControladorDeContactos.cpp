#include "../../headers/Controlador/ControladorDeContactos.h"
#include "../../headers/Modelo/Objetos/Figuras/Rectangulo.h"
#include "../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../headers/Modelo/Estados/Estados.h"
#include "../../headers/Modelo/Objetos/Personajes/Enemigo.h"

void Contacto::actualizarCharacters(std::list<Enemigo*> enemigos, std::list<Personaje*> personajes) {
	characters->clear();
	for (auto enemigo = enemigos.begin(); enemigo != enemigos.end(); enemigo++) {
		this->characters->push_back((Character*) (*enemigo));
	}
	for (auto personaje = personajes.begin(); personaje != personajes.end(); personaje++) {
		this->characters->push_back((Character*) (*personaje));
	}

}

void Contacto::BeginContact(b2Contact* contact) {

	this->actualizarCharacters(*enemigos, *personajes);

	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	//Hacemos que reaccionen las figuras
	figuraA->reaccionarCon(figuraB);
	figuraB->reaccionarCon(figuraA);

	//Controlo los contactos del personaje
	if ((figuraA->type == "personaje" || figuraA->type == "enemigo" )&& figuraB->esEstatico())
		((Character*) figuraA)->empiezoContacto(contact->GetFixtureA());

	if ((figuraB->type == "personaje" || figuraB->type == "enemigo") && figuraA->esEstatico())
		((Character*) figuraB)->empiezoContacto(contact->GetFixtureB());

}

void Contacto::EndContact(b2Contact* contact) {

	this->actualizarCharacters(*enemigos, *personajes);

	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	//Controlo los contactos del personaje
	if ((figuraA->type == "personaje" || figuraA->type == "enemigo" )&& figuraB->esEstatico()){
		((Character*) figuraA)->terminoContacto(contact->GetFixtureA());
	}

	if ((figuraB->type == "personaje" || figuraB->type == "enemigo") && figuraA->esEstatico()){
		((Character*) figuraB)->terminoContacto(contact->GetFixtureB());
	}
}

