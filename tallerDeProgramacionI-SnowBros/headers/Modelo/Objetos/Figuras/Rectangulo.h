class Rectangulo : public Figura{
private:
	int ancho;
	int alto;
	b2Body* body;
public:
	Rectangulo(int x, int y, int ancho, int alto, std::string color, int angulo, float masa, bool estatico);
	Rectangulo();
	~Rectangulo();
};
