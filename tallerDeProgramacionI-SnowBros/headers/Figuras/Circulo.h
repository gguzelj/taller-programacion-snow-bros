class Circulo : public Figura{
private:
	float escala;
	b2Body* body;
public:
	Circulo();
	Circulo(int x, int y, float escala, std::string color, int angulo, float masa, bool estatico);
	~Circulo();
};
