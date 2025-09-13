#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Ball;

// Rect
struct Rect {
    float x{ 0 }, y{ 0 }, width{ 0 }, height{ 0 };
};

class BallShooter : public GameObject {
public:
    // Canal del resorte
    Rect area{ 760, 120, 20, 600 };

    // Compresión/animación del pistón
    float comp{ 0.f }, prevComp{ 0.f };
    float compRate{ 1.4f };
    float releaseRate{ 2.6f };
    bool  releasing{ false };
    float maxSpeed{ 900.f };

    // Dibujador BRH
    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    // Física con la bola
    void ApplyToBall(class Ball& b);
    bool ContainsBall(const class Ball& b) const;
};
