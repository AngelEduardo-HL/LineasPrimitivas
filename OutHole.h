#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Ball; // fwd

class OutHole : public GameObject {
public:
    Rectangle area{ 350, 760, 100, 20 }; // borde inferior

    bool Check(const Ball& b) const; // solo declaración
    void Draw();
};
