#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Ball : public GameObject {
public:
	float vx{ 0 }, vy{ 0 }; // velocidad
	float r{ 16 }; // radio de la bola
	float restitution{ 0.70f }; // coeficiente de restitución ps el rebote
    Circulo drawer;

    void Update(float dt) override { (void)dt; }

    void Draw() override {
        // contorno BRH + relleno blanco para visibilidad
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
		drawer.DrawBRH(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r);// Contorno del Circulo con BRH
		drawer.FillBRH(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r, WHITE); // Circulo relleno
    }
};
