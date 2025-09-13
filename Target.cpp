#include "Target.h"
#include "Ball.h"
#include <cmath>

bool Target::CheckCollision(const Ball& b, CollisionInfo& info)
{
    float dx = b.tx - tx, dy = b.ty - ty;
    float rr = b.r + r;
    float d2 = dx * dx + dy * dy;
    if (d2 < rr * rr)
    {
        float d = (d2 > 1e-6f) ? std::sqrtf(d2) : rr;
        float nx = dx / d, ny = dy / d;

        info.nx = nx;
        info.ny = ny;
        info.penetration = rr - d;
        info.cx = tx + nx * r;
        info.cy = ty + ny * r;
        return true;
    }
    return false;
}

void Target::OnCollision(Ball& b, const CollisionInfo& info)
{
    // separar
    b.tx += info.nx * info.penetration;
    b.ty += info.ny * info.penetration;

    // rebote
    float vn = b.vx * info.nx + b.vy * info.ny;
    b.vx = b.vx - (1.0f + restitution) * vn * info.nx;
    b.vy = b.vy - (1.0f + restitution) * vn * info.ny;
}

void Target::Draw()
{
    Geometry::Mat3 I = Geometry::Traslacion(0, 0);
    drawer.DrawBRH(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r);
    if (filled) drawer.FillBRH(I, (int)(tx + 0.5f), (int)(ty + 0.5f), r, fill);
}
