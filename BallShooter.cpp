#include "BallShooter.h"
#include "Ball.h"
#include <cmath>

// compresión del pistón (SPACE)
void BallShooter::Update(float dt)
{
    prevComp = comp;

    // Mantener pulsado: comprime
    if (IsKeyDown(KEY_SPACE)) {
        releasing = false;
        comp += compRate * dt;
        if (comp > 1.f) comp = 1.f;
    }
    // Soltar
    if (IsKeyReleased(KEY_SPACE)) releasing = true;

    // Descomprimir hasta 0
    if (releasing) {
        comp -= releaseRate * dt;
        if (comp <= 0.f) { comp = 0.f; releasing = false; }
    }
}

void BallShooter::Draw()
{
    // Dibujo del marco del canal con BRH (contorno)
    Geometry::Mat3 I = Geometry::Traslacion(0, 0);
    int ax = (int)std::lround(area.x);
    int ay = (int)std::lround(area.y);
    int bx = (int)std::lround(area.x + area.width);
    int by = (int)std::lround(area.y + area.height);

    // Contorno del canal
    drawer.DrawBRH(I, ax, ay, bx, by);

    // Pistón (rectángulo relleno BRH) que baja al comprimir
    const float pistonH = 80.0f;
    float pistonY = area.y + comp * (area.height - pistonH);

    int X1 = (int)std::lround(area.x + 1);
    int Y1 = (int)std::lround(pistonY);
    int X2 = (int)std::lround(area.x + area.width - 1);
    int Y2 = (int)std::lround(pistonY + pistonH);

    // Relleno del pistón
    drawer.FillBRH(I, X1, Y1, X2, Y2, DARKGRAY);
}

bool BallShooter::ContainsBall(const Ball& b) const
{
    // AABB del canal
    float left = area.x + b.r;
    float right = area.x + area.width - b.r;
    float top = area.y;
    float bottom = area.y + area.height;
    return (b.tx >= left && b.tx <= right && b.ty >= top && b.ty <= bottom);
}

void BallShooter::ApplyToBall(Ball& b)
{
    if (!ContainsBall(b)) return;

    // Centrar la bola en X dentro del canal
    float cx = area.x + area.width * 0.5f;
    b.tx = cx;

	// Colision con el pistón
    const float pistonH = 80.0f;
    float pistonY = area.y + comp * (area.height - pistonH);
    float topFaceY = pistonY;

    // Mantener la bola encima del pistón y dentro del canal
    float minBallY = std::max(area.y + b.r, topFaceY - b.r);
    if (b.ty > minBallY) {
        b.ty = minBallY;
        if (b.vy > 0.0f) b.vy = 0.0f;
    }

    // Impulso durante descompresión
    if (releasing && prevComp > comp) {
        float dComp = (prevComp - comp);
        float boost = maxSpeed * dComp;
        if (boost > 0.0f) b.vy -= boost; // hacia arriba
    }
}
