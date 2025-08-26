#pragma once
#include <vector>
#include "Ball.h"
#include "Flipper.h"
#include "Target.h"
#include "BallShooter.h"
#include "OutHole.h"
#include "Cuadrado.h"
#include "Triangulo.h"
#include "Circulo.h"

class Game {
public:
    Game(int w, int h);

    void HandleInput();
    void Update(float dt);
    void Draw();

private:
    int W, H;

    // Entidades
    Ball ball;
    Flipper left, right;
    std::vector<Target> targets;
    BallShooter shooter;
    OutHole out;

    // Dibujadores extra (si quieres usar tus figuras para elementos del tablero)
    Cuadrado sq;
    Triangulo tr;
    Circulo  ci;

    // Físicas
    float gravity = 900.f;
    float wallRest = 0.70f;
    float friction = 0.995f;

    // Estado
    int score = 0;
    int lives = 3;

    // helpers
    void CollideWalls();
    void CollideFlippers();
    void CollideTargets();
};
