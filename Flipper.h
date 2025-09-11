#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Flipper : public GameObject {
public:
    float length{ 150.f };
    float width{ 20.f };

	// �ngulos y velocidad
	float restAngle{ -25.f };   // �ngulo de reposo
	float hitAngle{ 30.f };   // �ngulo al pulsar
    float speed{ 600.f };
    bool  pressed{ false };

    Color color{ DARKBLUE };      // color visual
    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    void Segment(float& ax, float& ay, float& bx, float& by) const;

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
