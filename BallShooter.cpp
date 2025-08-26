#include "BallShooter.h"
#include "Ball.h"
#include "raylib.h"
#include <cmath>

void BallShooter::Update(float dt)
{
    prevComp = comp;

    // Mantener SPACE para comprimir
    if (IsKeyDown(KEY_SPACE)) {
        releasing = false;
        comp += compRate * dt;
        if (comp > 1.f) comp = 1.f;
    }
    // Al SOLTAR, comienza la descompresi�n
    if (IsKeyReleased(KEY_SPACE)) {
        releasing = true;
    }

    if (releasing) {
        comp -= releaseRate * dt;
        if (comp <= 0.f) { comp = 0.f; releasing = false; }
    }
}

void BallShooter::Draw()
{
    // Marco del canal
    DrawRectangleLines((int)area.x, (int)area.y, (int)area.width, (int)area.height, RAYWHITE);

    // Pist�n: un rect�ngulo que sube al comprimirse
    // Altura base del pist�n (visual)
    const float pistonH = 80.f;
    float pistonY = area.y + comp * (area.height - pistonH);
    float pistonX1 = area.x + 1;
    float pistonX2 = area.x + area.width - 1;

    // Dibuja pist�n con tu Cuadrado
    int X1 = (int)std::round(pistonX1);
    int Y1 = (int)std::round(pistonY);
    int X2 = (int)std::round(pistonX2);
    int Y2 = (int)std::round(pistonY + pistonH);

    Geometry::Mat3 I = Geometry::Traslacion(0, 0);
    drawer.DrawDDA(I, X1, Y1, X2, Y2);

    // Barra de carga a la izquierda del canal
    int barW = 6;
    int barX = (int)area.x - barW - 4;
    int barY = (int)area.y;
    int barH = (int)area.height;
    DrawRectangleLines(barX, barY, barW, barH, RAYWHITE);
    int fillH = (int)std::round(barH * comp);
    DrawRectangle(barX + 1, barY + barH - fillH + 1, barW - 2, fillH - 2, Color{ 80,200,120,180 });
}

void BallShooter::ApplyToBall(Ball& b)
{
    // Solo si la bola est� dentro del canal
    if (!Contains(b.tx, b.ty)) return;

    // Mientras se descomprime, si comp decrece, transfiero velocidad
    if (releasing && prevComp > comp) {
        float dComp = (prevComp - comp);     // cu�nto "cedi�" el resorte este frame
        float boost = maxSpeed * dComp;      // parte proporcional
        // Empuje hacia ARRIBA
        if (boost > 0.f) {
            b.vy -= boost;
        }
    }

    // Mantener la bola �encolumna� en el centro del canal horizontalmente
    float cx = area.x + area.width * 0.5f;
    b.tx = cx;
}
