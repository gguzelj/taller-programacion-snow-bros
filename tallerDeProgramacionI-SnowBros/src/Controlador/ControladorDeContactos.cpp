#include "../../headers/Controlador/ControladorDeContactos.h"
#include "../../headers/Modelo/Objetos/Figuras/Rectangulo.h"
#include "../../headers/Modelo/Objetos/Personajes/Personaje.h"
#include "../../headers/Modelo/Estados/Estados.h"
#include "../../headers/Modelo/Objetos/Personajes/Enemigo.h"

void Contacto::BeginContact(b2Contact* contact) {

	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	//Hacemos que reaccionen las figuras
	figuraA->beginContact(figuraB, contact);
	figuraB->beginContact(figuraA, contact);

}

void Contacto::EndContact(b2Contact* contact) {

	Figura *figuraA = (Figura*) contact->GetFixtureA()->GetUserData();
	Figura *figuraB = (Figura*) contact->GetFixtureB()->GetUserData();

	//Hacemos que reaccionen las figuras
	figuraA->endContact(figuraB, contact);
	figuraB->endContact(figuraA, contact);

}
