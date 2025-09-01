#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Flipper : public GameObject {
public:
    float length{150.f};
    float width{20.f};

    // Como antes: reposo ARRIBA, al presionar BAJAN
    float restAngle{ -25.f };   // izq: -25; der: +25 (ver constructor en Game.cpp)
    float hitAngle  {  30.f };  // izq: +30; der: -30
    float speed{600.f};
    bool  pressed{false};

    // color visual del flipper
    Color color{ BLUE };

    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    void Segment(float& ax, float& ay, float& bx, float& by) const;

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
