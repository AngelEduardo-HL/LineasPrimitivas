#include "Wall.h"
#include "Ball.h"
#include "Geometry.h"
#include <cmath>

static inline float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

Wall Wall::FromCenter(float cx, float cy, float angDeg, float length,
    float thick, Color c, float rest)
{
    float rad = angDeg * PI / 180.0f;
    float hx = 0.5f * length * cosf(rad);
    float hy = 0.5f * length * sinf(rad);
    return FromAB(cx - hx, cy - hy, cx + hx, cy + hy, thick, c, rest);
}

void Wall::Draw() const
{
    float L = std::sqrtf((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
    float angDeg = std::atan2f(by - ay, bx - ax) * 180.0f / PI;

    Geometry::Mat3 R = Geometry::RotacionPivote(angDeg, (int)ax, (int)ay);

    // Rect local [ax..ax+L] x [ay - t/2 .. ay + t/2]
    drawer.FillBRH(R,
        (int)ax, (int)(ay - thick * 0.5f),
        (int)(ax + L), (int)(ay + thick * 0.5f),
        color);
}

bool Wall::CollideAndBounce(Ball& b) const
{
    // Broad-phase AABB entero (expansivo para no perder colisiones)
    float radCaps = thick * 0.5f - 1.f; // -inset del fill
    int xmin = (int)std::floor((ax < bx ? ax : bx) - radCaps - b.r);
    int xmax = (int)std::ceil((ax > bx ? ax : bx) + radCaps + b.r);
    int ymin = (int)std::floor((ay < by ? ay : by) - radCaps - b.r);
    int ymax = (int)std::ceil((ay > by ? ay : by) + radCaps + b.r);

    Geometry::AABB aWall = Geometry::MakeAABB(xmin, ymin, xmax, ymax);
    Geometry::AABB aBall = Geometry::MakeAABB((int)(b.tx - b.r), (int)(b.ty - b.r),
        (int)(b.tx + b.r), (int)(b.ty + b.r));
    if (!Geometry::AABBOverlap(aBall, aWall)) return false;

    // Colisión exacta: proyección del centro en el segmento
    float vx = bx - ax, vy = by - ay;
    float wx = b.tx - ax, wy = b.ty - ay;
    float vv = vx * vx + vy * vy;
    float t = (vv > 0.f) ? (wx * vx + wy * vy) / vv : 0.f;
    t = clampf(t, 0.f, 1.f);
    float qx = ax + t * vx, qy = ay + t * vy;

    float dx = b.tx - qx, dy = b.ty - qy;
    float eff = b.r + radCaps;
    float d2 = dx * dx + dy * dy;
    float ee = eff * eff;

    if (d2 < ee) {
        float d = (d2 > 1e-8f) ? std::sqrtf(d2) : 0.f;
        float nx = (d > 1e-8f) ? dx / d : 0.f;
        float ny = (d > 1e-8f) ? dy / d : -1.f;
        float pen = eff - d;

        // Separar
        b.tx += nx * pen;
        b.ty += ny * pen;

        // Rebote
        float vn = b.vx * nx + b.vy * ny;
        b.vx = b.vx - (1.f + restitution) * vn * nx;
        b.vy = b.vy - (1.f + restitution) * vn * ny;
        return true;
    }
    return false;
}
