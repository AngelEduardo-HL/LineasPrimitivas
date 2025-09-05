#pragma once
#include "GameObject.h"
#include "Circulo.h"

class Target : public GameObject {
public:
    float r{ 20.f };
    float restitution{ 1.10f };
    int   score{ 0 };          // rojo=100, amarillo=50, verde=25 bumpers=0
	Color ring{ RAYWHITE };    // color del aro
    bool  filled{ false };
    Color fill{ WHITE };       // color de relleno si filled=true

    Circulo drawer;

    void Draw() override;

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
