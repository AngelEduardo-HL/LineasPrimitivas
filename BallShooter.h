#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Ball;

class BallShooter : public GameObject {
public:
    // Canal del shooter (vertical a la derecha)
    Rectangle area{ 760, 120, 20, 600 };

    // Compresi�n del �resorte� (0..1)
    float comp{ 0.f };            // compresi�n actual
    float prevComp{ 0.f };        // compresi�n del frame anterior
    float compRate{ 1.4f };       // velocidad de compresi�n (al presionar SPACE)
    float releaseRate{ 2.6f };    // velocidad de descompresi�n (al soltar)
    bool  releasing{ false };     // est� descomprimiendo

    // potencia
    float maxSpeed{ 900.f };

    // Dibujo
    Cuadrado drawer;

    void Update(float dt) override;
    void Draw() override;

    // Aplica impulso a la bola mientras el resorte se descomprime
    void ApplyToBall(Ball& b);

    bool Contains(float x, float y) const {
        return (x >= area.x && x <= area.x + area.width &&
            y >= area.y && y <= area.y + area.height);
    }
};
