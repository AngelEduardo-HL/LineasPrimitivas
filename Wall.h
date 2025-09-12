#pragma once
#include "Cuadrado.h"
class Ball;

struct Wall {
    // Segmento central de la pared
    float ax{ 0 }, ay{ 0 }; // punto A (inicio)
    float bx{ 0 }, by{ 0 }; // punto B (fin)
    float thick{ 14.f };  // grosor visual y de colisión
    float restitution{ 0.98f };
    Color color{ BLACK };

    Cuadrado drawer; // para FillBRH

    // Dibujo
    void Draw();

    // Colisión círculo
    bool CollideAndBounce(Ball& b) const;

    // Helpers de construcción
    static Wall FromAB(float ax, float ay, float bx, float by,
        float thick, Color c, float rest = 0.98f) {
        Wall w; w.ax = ax; w.ay = ay; w.bx = bx; w.by = by; w.thick = thick; w.color = c; w.restitution = rest; return w;
    }
    static Wall FromCenter(float cx, float cy, float angDeg, float length,
        float thick, Color c, float rest = 0.98f) {
        float rad = angDeg * PI / 180.0f;
        float hx = 0.5f * length * cosf(rad);
        float hy = 0.5f * length * sinf(rad);
        return FromAB(cx - hx, cy - hy, cx + hx, cy + hy, thick, c, rest);
    }
};
