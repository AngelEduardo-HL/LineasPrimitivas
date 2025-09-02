#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Flipper : public GameObject {
public:
    float length{ 150.f };
    float width{ 20.f };

    // Versión que funcionaba: reposo ARRIBA, al presionar BAJA
    float restAngle{ -25.f };   // izq: -25 ; der: +25 (ver Game.cpp)
    float hitAngle{ 30.f };   // izq: +30 ; der: -30
    float speed{ 600.f };
    bool  pressed{ false };

    Color color{ BLUE };      // color visual
    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    void Segment(float& ax, float& ay, float& bx, float& by) const;

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
