#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Ball;

class BallShooter : public GameObject {
public:
    Rectangle area{ 760, 120, 20, 600 };

    float comp{ 0.f }, prevComp{ 0.f };
    float compRate{ 1.4f };
    float releaseRate{ 2.6f };
    bool  releasing{ false };
    float maxSpeed{ 900.f };

    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    void ApplyToBall(Ball& b);
    bool ContainsBall(const Ball& b) const;
};
