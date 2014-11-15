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

	//Controlo los contactos del personaje
	if ((figuraA->type == ID_PERSONAJE || figuraA->type == ID_ENEMIGO )&& figuraB->esEstatico()){
		((Character*) figuraA)->terminoContacto(contact->GetFixtureA());
	}

	if ((figuraB->type == ID_PERSONAJE || figuraB->type == ID_ENEMIGO) && figuraA->esEstatico()){
		((Character*) figuraB)->terminoContacto(contact->GetFixtureB());
	}


	//Esta villereada la voy a sacar.. otro dia
	if (figuraB->type == ID_PERSONAJE && figuraA->type == ID_ENEMIGO)
		((Personaje*) figuraB)->empujando = false;

	if (figuraA->type == ID_PERSONAJE && figuraB->type == ID_ENEMIGO)
		((Personaje*) figuraA)->empujando = false;


}
