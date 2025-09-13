#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Ball;

class OutHole : public GameObject {
public:
    int x{ 350 }, y{ 760 }, w{ 120 }, h{ 20 };

    bool Check(const Ball& b) const;

    void Draw();

    inline int Left()   const { return x; }
    inline int Right()  const { return x + w; }
    inline int Top()    const { return y; }
    inline int Bottom() const { return y + h; }

private:
    Cuadrado drawer;
};
