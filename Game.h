#pragma once
#include <vector>
#include "Ball.h"
#include "Flipper.h"
#include "Target.h"
#include "BallShooter.h"
#include "OutHole.h"
#include "Triangulo.h"
#include "Cuadrado.h"
#include "Wall.h"

// Guía inclinada a la salida del shooter
struct ShooterGuide {
    float ax{ 0 }, ay{ 0 }, bx{ 0 }, by{ 0 };
    float thick{ 14.f };
    float restitution{ 0.95f };
    Cuadrado drawer;

    void Draw() {
        // Longitud y ángulo del segmento
        float L = sqrtf((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
        float angDeg = atan2f(by - ay, bx - ax) * 180.0f / PI;
        // Matriz de rotación
        Geometry::Mat3 R = Geometry::RotacionPivote(angDeg, (int)ax, (int)ay);
        // Relleno del rectángulo (BRH)
        drawer.FillBRH(R, (int)ax, (int)(ay - thick * 0.5f), (int)(ax + L), (int)(ay + thick * 0.5f), GRAY);
    }

    bool CollideAndBounce(class Ball& b) const;
};

struct TriDeflector {
    int x1, y1, x2, y2, x3, y3;
    float restitution{ 0.75f };
    void Draw(Triangulo& tr) const {
        Geometry::Mat3 I = Geometry::Traslacion(0, 0);
        tr.DrawBRH(I, x1, y1, x2, y2, x3, y3);
		tr.FillBRH(I, x1, y1, x2, y2, x3, y3, GREEN);
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
    std::vector<Target> targets;   // incluye bumpers grises
    std::vector<TriDeflector> tris;
	std::vector<Wall> walls; // bordes del campo

    BallShooter shooter;
    ShooterGuide guide;
    OutHole out;
    Triangulo tr;

    float gravity = 900.f;
    float friction = 0.995f;

    int score = 0;
    int lives = 3;

    void CollideBorders();
    void CollideFlippers();
    void CollideTargets();
    void CollideTriangles();
};
