#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
#include <iostream>
#include <Box2D/Box2D.h>
#include "../headers/Parser/JsonParser.h"

class Controlador{
public:
	Controlador(){};
	~Controlador(){};

	/*
	 *En base a un archivo JSON, crea los objetos del modelo world
	 */
	bool setWorldObjects(std::string jsonPath,b2World* world);

	/*
	 * En base a un archivo JSON, inicializa la ventana en donde se representara el modelo, en base a los parametros
	 * (alto_px, ancho_px, imagePath) indicados por el archivo
	 */
	bool initWindow(std::string jsonPath, Drawer* drawer);

};



#endif /* CONTROLADOR_H_ */
