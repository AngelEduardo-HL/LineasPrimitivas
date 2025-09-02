#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Target : public GameObject {
public:
    float r{ 20.f };
    float restitution{ 1.10f };
    int   score{ 0 };          // rojo=100, amarillo=50, verde=25; bumpers=0
    Color ring{ RAYWHITE };    // aro
    bool  filled{ false };
    Color fill{ WHITE };       // color de relleno si filled=true

    Circulo drawer;

    void Draw() override {
        // aro visible
        DrawCircleLines((int)tx, (int)ty, (int)r, ring);
        if (filled) DrawCircle((int)tx, (int)ty, (int)(r * 0.7f), fill);
        // opcional: perfilar con BRH
        // Geometry::Mat3 I = Geometry::Traslacion(0,0);
        // drawer.DrawBRH(I, (int)(tx+0.5f), (int)(ty+0.5f), r);
    }

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
