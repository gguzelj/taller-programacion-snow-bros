/*
 * Clase de la cual van a heredar todos los tipos de figuras (Rectangulo, Poligono, Circulo, Parelelogramo)
 * Dejamos la idea de si Personaje deberia heredar de Figura o no. Osea, en realidad es un figura dinamica.
 */
class Figura{
private:
	int x;
	int y;
	std::string color;
	int angulo;
	float masa;
	bool estatico;
public:
	Figura(int x, int y, std::string color, int angulo, float masa, bool estatico);
	Figura();
	virtual ~Figura();


};
