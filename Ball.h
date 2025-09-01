#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Ball : public GameObject {
public:
    float vx{ 0 }, vy{ 0 };
    float r{ 6 };
    float mass{ 1.0f };
    float restitution{ 0.70f };

    Circulo drawer;

    void Update(float dt) override { (void)dt; }
    void Draw() override {
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
        drawer.DrawBRH(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r);
    }
};
