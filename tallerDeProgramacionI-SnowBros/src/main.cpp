/*
 * main.cpp
 *
 *  Created on: Aug 31, 2014
 *      Author: lmasello
 */
#include "../headers/ventanaGrafica.h"
#include "../headers/Viewer.h"

int main(int argc,char* argv[]){

	//Aca habria que cargar en memoria al JSON

	//Inicializar la ventana
	VentanaGrafica* window = new VentanaGrafica();
	Viewer* viewer = window;
	(*viewer).reproducirVentana();

	delete viewer;

	return 0;
}
