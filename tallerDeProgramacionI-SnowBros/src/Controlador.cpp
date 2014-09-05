#include "../headers/Controlador.h"

bool Controlador::setWorldObjects(std::string jsonPath,b2World* world){
	return true;
}

bool Controlador::initWindow(std::string jsonPath, Drawer* drawer){
	JsonParser* parser = new JsonParser(jsonPath);

//	int ancho_px = parser.getAncho_px();
//	int alto_px = parser.getAlto_px();
//	std::string imagePath = parser.getImagePath();

	int ancho_px = ANCHO_PX;
	int alto_px = ALTO_PX;
	std::string imagePath = "resources/Everest.png";

	//Inicializa al drawer, en caso de algun error devuelve false
	try{
		drawer->runWindow(ancho_px ,alto_px ,imagePath);
	}
	catch (int e){
		return false;
	}

	delete parser;
	return true;
}

