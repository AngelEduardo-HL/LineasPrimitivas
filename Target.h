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
	// si se rellena
	Color fill{ WHITE };         // color del relleno

    Circulo drawer; // Dibujado y relleno

    void Draw() override;

    bool CheckCollision(const class Ball& b, CollisionInfo& info) override;
    void OnCollision(class Ball& b, const CollisionInfo& info) override;
};
