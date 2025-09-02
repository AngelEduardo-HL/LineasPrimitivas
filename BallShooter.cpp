#include "BallShooter.h"
#include "Ball.h"
#include "raylib.h"
#include <cmath>

void BallShooter::Update(float dt)
{
    prevComp = comp;

    if (IsKeyDown(KEY_SPACE)) { releasing = false; comp += compRate * dt; if (comp > 1.f) comp = 1.f; }
    if (IsKeyReleased(KEY_SPACE)) releasing = true;
    if (releasing) { comp -= releaseRate * dt; if (comp <= 0.f) { comp = 0.f; releasing = false; } }
}

void BallShooter::Draw()
{
    DrawRectangleLines((int)area.x, (int)area.y, (int)area.width, (int)area.height, BLACK);

    // pistón (BRH) que baja al comprimir
    const float pistonH = 80.0f;
    float pistonY = area.y + comp * (area.height - pistonH);

    int X1 = (int)std::round(area.x + 1);
    int Y1 = (int)std::round(pistonY);
    int X2 = (int)std::round(area.x + area.width - 1);
    int Y2 = (int)std::round(pistonY + pistonH);

    Geometry::Mat3 I = Geometry::Traslacion(0, 0);
    drawer.FillBRH(I, X1, Y1, X2, Y2);
}

bool BallShooter::ContainsBall(const Ball& b) const
{
    float left = area.x + b.r;
    float right = area.x + area.width - b.r;
    float top = area.y;
    float bottom = area.y + area.height;
    return (b.tx >= left && b.tx <= right && b.ty >= top && b.ty <= bottom);
}

void BallShooter::ApplyToBall(Ball& b)
{
    if (!ContainsBall(b)) return;

    float cx = area.x + area.width * 0.5f;
    b.tx = cx;

    const float pistonH = 80.0f;
    float pistonY = area.y + comp * (area.height - pistonH);
    float topFaceY = pistonY;

    // clamp: bola por encima del pistón y dentro del canal
    float minBallY = std::max(area.y + b.r, topFaceY - b.r);
    if (b.ty > minBallY) {
        b.ty = minBallY;
        if (b.vy > 0.0f) b.vy = 0.0f;
    }

    // impulso al descomprimir
    if (releasing && prevComp > comp) {
        float dComp = (prevComp - comp);
        float boost = maxSpeed * dComp;
        if (boost > 0.0f) b.vy -= boost;
    }
}
