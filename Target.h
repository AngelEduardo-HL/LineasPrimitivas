#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Target : public GameObject {
public:
    float r{ 22.f };
    float restitution{ 1.05f };
    int   score{ 100 };

    Circulo drawer;

    void Draw() override {
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
        drawer.DrawDDA(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r);
    }

    bool CheckCollision(const Ball& b, CollisionInfo& info) override;
    void OnCollision(Ball& b, const CollisionInfo& info) override;
};
