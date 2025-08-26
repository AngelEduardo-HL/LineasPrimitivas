#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Ball : public GameObject {
public:
    float vx{ 0 }, vy{ 0 };
    float r{ 12 };
    float mass{ 1.0f };
    float restitution{ 0.70f };

    Circulo drawer; // usa tu clase para dibujar

    void Update(float dt) override {
        // gravedad simple (se aplica en Game para poder pausar si quieres)
        (void)dt;
    }

    void Draw() override {
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
        drawer.DrawDDA(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r);
    }
};
