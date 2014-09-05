class Poligono : public Figura{
private:
	float escala;
	b2Body* body;
public:
	Poligono(int x, int y, float escala, std::string color, int angulo, float masa, bool estatico);
	Poligono();
	~Poligono();
};
