#include "../../headers/Parser/FileReader.h"

/**
 * Clase encargada de cargar el archivo json en un string
 */
std::string FileReader::read(std::string path) {

	//EL archivo ya deberia estar cargado por alguna otra clase. Esto hay que borrarlo!
	std::ifstream t(path);
	std::string str;

	if (t.is_open()) {
		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
	}

	return str;
}
