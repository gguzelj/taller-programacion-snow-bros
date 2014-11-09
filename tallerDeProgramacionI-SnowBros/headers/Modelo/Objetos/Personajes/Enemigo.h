#ifndef ENEMIGO_H_
#define ENEMIGO_H_

#include "Character.h"

#define MITAD_ANCHO_ENEMIGO 0.6f
#define MITAD_ALTO_ENEMIGO 0.85
#define IZQUIERDA 'l'
#define DERECHA 'r'

class Enemigo: public Character {
private:

	//bool estaCongelado;
	int nivelDeCongelamiento;


	time_t tiempoDeImpactoDeLaUltimaBola;
public:


	Enemigo(float x, float y, b2World* world);
	~Enemigo();
	void disparar();

	void handleInput(SDL_Keycode input, Uint32 input_type);

	void reaccionarCon(Figura* figura);

	void reaccionarConBolaNieve(BolaNieve* bola);

	void congelar();

	int getNivelDeCongelamiento(){
		return nivelDeCongelamiento;
	}

};

#endif /* ENEMIGO_H_ */
