#pragma once
#include <vector>
#include "Ball.h"
#include "Flipper.h"
#include "Target.h"
#include "BallShooter.h"
#include "OutHole.h"
#include "Triangulo.h"

struct TriDeflector {
    int x1, y1, x2, y2, x3, y3;
    float restitution{ 0.75f };
    void Draw(Triangulo& tr) const {
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
        tr.DrawBRH(I, x1, y1, x2, y2, x3, y3);
    }
};

class Game {
public:
    Game(int w, int h);

    void HandleInput();
    void Update(float dt);
    void Draw();

private:
    int W, H;

    Ball ball;
    Flipper left, right;
    std::vector<Target> targets;   // incluye bumpers (score=0, grises)
    std::vector<TriDeflector> tris;

    BallShooter shooter;
    OutHole out;
    Triangulo tr; // para dibujar tris

    float gravity = 900.f;
    float friction = 0.995f;

    int score = 0;
    int lives = 3;

    // colisiones
    void CollideBorders();     // <-- solo bordes del campo
    void CollideFlippers();
    void CollideTargets();
    void CollideTriangles();
};
