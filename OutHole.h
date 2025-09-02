#pragma once
#include "GameObject.h"

class Ball;

class OutHole : public GameObject {
public:
    Rectangle area{ 350, 760, 120, 20 };
    bool Check(const Ball& b) const;
    void Draw();
};
