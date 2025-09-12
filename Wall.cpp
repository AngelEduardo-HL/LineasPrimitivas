#include "Wall.h"
#include "Ball.h"
#include <cmath>

static inline float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

void Wall::Draw()
{
    float L = std::sqrtf((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
    float angDeg = std::atan2f(by - ay, bx - ax) * 180.0f / PI;

    Geometry::Mat3 R = Geometry::RotacionPivote(angDeg, (int)ax, (int)ay);

    // Rectángulo local
    drawer.FillBRH(R,
        (int)ax, (int)(ay - thick * 0.5f),
        (int)(ax + L), (int)(ay + thick * 0.5f),
        color);
    // (Si quieres contorno extra) -> drawer.DrawBRH(R, ...)
}

bool Wall::CollideAndBounce(Ball& b) const
{
    // Proyección del centro de la bola sobre el segmento A->B
    float vx = bx - ax, vy = by - ay;
    float wx = b.tx - ax, wy = b.ty - ay;
    float vv = vx * vx + vy * vy;
    float t = (vv > 0.f) ? (wx * vx + wy * vy) / vv : 0.f;
    t = clampf(t, 0.f, 1.f);
    float qx = ax + t * vx, qy = ay + t * vy;

    float dx = b.tx - qx, dy = b.ty - qy;
    float d = std::sqrtf(dx * dx + dy * dy);

    
    float eff = b.r + (thick * 0.5f - 1.f);

    if (d < eff) {
        float nx = (d > 1e-6f) ? dx / d : 0.f;
        float ny = (d > 1e-6f) ? dy / d : -1.f;
        float pen = eff - d;

        // separar
        b.tx += nx * pen;
        b.ty += ny * pen;

        // rebote
        float vn = b.vx * nx + b.vy * ny;
        b.vx = b.vx - (1.f + restitution) * vn * nx;
        b.vy = b.vy - (1.f + restitution) * vn * ny;
        return true;
    }
    return false;
}
