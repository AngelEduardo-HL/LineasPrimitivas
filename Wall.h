#pragma once
#include "raylib.h"
#include "Cuadrado.h"
class Ball; // forward

struct Wall {
    // Segmento central de la pared
    float ax{ 0 }, ay{ 0 };
    float bx{ 0 }, by{ 0 };
    float thick{ 14.f };
    float restitution{ 0.5f };
    Color color{ BLACK };

	mutable Cuadrado drawer; // para dibujar la pared

    void Draw() const;
    bool CollideAndBounce(Ball& b) const;

    static Wall FromAB(float ax, float ay, float bx, float by,
        float thick, Color c, float rest = 0.98f) {
        Wall w; w.ax = ax; w.ay = ay; w.bx = bx; w.by = by; w.thick = thick; w.color = c; w.restitution = rest; return w;
    }
    static Wall FromCenter(float cx, float cy, float angDeg, float length,
        float thick, Color c, float rest = 0.98f);
};
