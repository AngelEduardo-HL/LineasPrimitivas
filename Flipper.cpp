#include "Flipper.h"
#include "Ball.h"
#include <cmath>

static inline float Dot(float ax, float ay, float bx, float by) { return ax * bx + ay * by; }
static inline float Clamp(float v, float lo, float hi) { return (v < lo) ? lo : ((v > hi) ? hi : v); }
static inline float Len(float x, float y) { return std::sqrtf(x * x + y * y); }

static float DistPointSeg(float px, float py, float ax, float ay, float bx, float by, float& qx, float& qy)
{
    float vx = bx - ax, vy = by - ay;
    float wx = px - ax, wy = py - ay;
    float vv = vx * vx + vy * vy;
    float t = (vv > 0) ? (wx * vx + wy * vy) / vv : 0.f;
    t = Clamp(t, 0.f, 1.f);
    qx = ax + t * vx; qy = ay + t * vy;
    float dx = px - qx, dy = py - qy;
    return std::sqrtf(dx * dx + dy * dy);
}

bool Flipper::CheckCollision(const Ball& b, CollisionInfo& info)
{
    float ax, ay, bx, by; Segment(ax, ay, bx, by);
    float qx, qy;
    float d = DistPointSeg(b.tx, b.ty, ax, ay, bx, by, qx, qy);

    // Añadimos "slop" para estabilizar (2 px)
    const float slop = 2.0f;
    float rad = b.r + width * 0.5f + slop;  // cápsula más gruesa

    if (d < rad)
    {
        // normal desde el segmento hacia la bola
        float nx = b.tx - qx, ny = b.ty - qy;
        float nlen = Len(nx, ny); if (nlen < 1e-4f) return false;
        nx /= nlen; ny /= nlen;

        info.nx = nx; info.ny = ny;
        info.penetration = rad - d;   // incluye slop
        info.cx = qx; info.cy = qy;
        return true;
    }
    return false;
}

void Flipper::OnCollision(Ball& b, const CollisionInfo& info)
{
    // Corrige penetración
    b.tx += info.nx * info.penetration;
    b.ty += info.ny * info.penetration;

    // Reflejo con restitución fuerte
    const float restitution = 0.95f;
    float vn = b.vx * info.nx + b.vy * info.ny;
    b.vx = b.vx - (1.0f + restitution) * vn * info.nx;
    b.vy = b.vy - (1.0f + restitution) * vn * info.ny;

    // Impulso extra si está presionado
    if (pressed) {
        const float bonus = 160.f; // un poco más
        b.vx += info.nx * bonus;
        b.vy += info.ny * bonus;
    }
}