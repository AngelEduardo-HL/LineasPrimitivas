#include "Flipper.h"
#include "Ball.h"
#include "raylib.h"
#include <cmath>

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

void Flipper::Update(float dt)
{
    float target = pressed ? hitAngle : restAngle;
    if (rotDeg < target) { rotDeg += speed * dt; if (rotDeg > target) rotDeg = target; }
    else if (rotDeg > target) { rotDeg -= speed * dt; if (rotDeg < target) rotDeg = target; }
}

void Flipper::Draw()
{
	// Relleno de color para dar color al flipper con BRH
	Cuadrado& drawer = this->drawer; // evitar shadowing
  
    // Perfil con líneas BRH
    int X1 = (int)(px + 0.5f);
    int Y1 = (int)(py - width * 0.5f + 0.5f);
    int X2 = (int)(px + length + 0.5f);
    int Y2 = (int)(py + width * 0.5f + 0.5f);
    drawer.FillBRH(BuildM(), X1, Y1, X2, Y2, color);
}

void Flipper::Segment(float& ax, float& ay, float& bx, float& by) const //
{
    Geometry::Mat3 M = const_cast<Flipper*>(this)->BuildM();
    int Axi = (int)(px + 0.5f), Ayi = (int)(py + 0.5f);
    int Bxi = (int)(px + length + 0.5f), Byi = (int)(py + 0.5f);
    int Axr, Ayr, Bxr, Byr;
    Geometry::TransformPoint(M, Axi, Ayi, Axr, Ayr);
    Geometry::TransformPoint(M, Bxi, Byi, Bxr, Byr);
    ax = (float)Axr; ay = (float)Ayr; bx = (float)Bxr; by = (float)Byr;
}

bool Flipper::CheckCollision(const Ball& b, CollisionInfo& info)
{
    float ax, ay, bx, by; Segment(ax, ay, bx, by);
    float qx, qy;
    float d = DistPointSeg(b.tx, b.ty, ax, ay, bx, by, qx, qy);

	const float slop = 1.0f; // margen extra para evitar que se atasque
    float rad = b.r + width * 1.5f + slop; 

    if (d < rad)
    {
        float nx = b.tx - qx, ny = b.ty - qy;
        float nlen = Len(nx, ny); if (nlen < 1e-4f) return false;
        nx /= nlen; ny /= nlen;

        info.nx = nx; info.ny = ny;
        info.penetration = rad - d;
        info.cx = qx; info.cy = qy;
        return true;
    }
    return false;
}

void Flipper::OnCollision(Ball& b, const CollisionInfo& info)
{
    b.tx += info.nx * info.penetration;
    b.ty += info.ny * info.penetration;

	const float restitution = 0.95f; // Rebote grande por ser Flippers
    float vn = b.vx * info.nx + b.vy * info.ny;
    b.vx = b.vx - (1.0f + restitution) * vn * info.nx;
    b.vy = b.vy - (1.0f + restitution) * vn * info.ny;

    if (pressed) {
        const float bonus = 180.f;
        b.vx += info.nx * bonus;
        b.vy += info.ny * bonus;
    }
}
