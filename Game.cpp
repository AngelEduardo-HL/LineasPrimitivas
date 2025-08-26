#include "Game.h"
#include "raylib.h"
#include <cmath>

static inline float Clamp(float v, float lo, float hi) { return (v < lo) ? lo : ((v > hi) ? hi : v); }

Game::Game(int w, int h) : W(w), H(h)
{
    // Shooter/OutHole
    shooter.area = { (float)(W - 40), 120.f, 20.f, 600.f };
    out.area = { (float)(W / 2 - 60), (float)(H - 20), 120.f, 18.f };

    // Bola: nasce dentro del canal, abajo a la derecha (quieta)
    ball.r = 5.f;
	ball.tx = shooter.area.x + shooter.area.width * 0.5f; // centro X del canal
	ball.ty = shooter.area.y + shooter.area.height - ball.r - 790.f; // justo encima del pistón
    ball.vx = 0.f;
    ball.vy = 0.f;  

    // Flippers
    left.px = 220.f; left.py = 680.f; left.length = 140.f; left.width = 18.f;
    left.restAngle = -25.f; left.hitAngle = 35.f; left.rotDeg = left.restAngle;

    right.px = 580.f; right.py = 680.f; right.length = -140.f; right.width = 18.f;
    right.restAngle = 25.f; right.hitAngle = -35.f; right.rotDeg = right.restAngle;

    // Targets/Bumpers
    targets.resize(3);
    targets[0].tx = 400.f; targets[0].ty = 300.f; targets[0].r = 26.f; targets[0].restitution = 1.05f; targets[0].score = 150;
    targets[1].tx = 500.f; targets[1].ty = 380.f; targets[1].r = 22.f; targets[1].restitution = 1.05f; targets[1].score = 100;
    targets[2].tx = 300.f; targets[2].ty = 380.f; targets[2].r = 22.f; targets[2].restitution = 1.05f; targets[2].score = 100;

    // Shooter/OutHole (por defecto en la derecha e inferior)
    shooter.area = { (float)(W - 40), 120.f, 20.f, 600.f };
    out.area = { (float)(W / 2 - 60), (float)(H - 20), 120.f, 18.f };
}

void Game::HandleInput()
{
    left.pressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    right.pressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    // recargar bola rápido
    if (IsKeyPressed(KEY_R)) {
        ball.tx = 400.f; ball.ty = 120.f; ball.vx = 120.f; ball.vy = -50.f;
    }
}

void Game::Update(float dt)
{
    HandleInput();

    // Sub–stepping para evitar "túnel" y solapamientos grandes
    const float hz = 120.f; // 120 Hz
    int steps = (int)ceilf(dt * hz);
    if (steps < 1) steps = 1;
    float h = dt / steps;

    // Fricción por subpaso
    float fricStep = powf(friction, 1.0f / steps);

    for (int i = 0; i < steps; ++i)
    {
        // Shooter: comprime/desc. y aplica impulso a la bola
        shooter.Update(h);
        shooter.ApplyToBall(ball);

        // Flippers
        left.Update(h);
        right.Update(h);

        // Bola (gravedad + integración)
        ball.vy += gravity * h;
        ball.tx += ball.vx * h;
        ball.ty += ball.vy * h;

        CollideWalls();
        CollideFlippers();
        CollideTargets();

        // fricción leve por subpaso
        ball.vx *= fricStep;
        ball.vy *= fricStep;

        // OutHole
        if (out.Check(ball)) {
            lives--;
            // Reinicia bola dentro del canal
            ball.tx = shooter.area.x + shooter.area.width * 0.5f;
            ball.ty = shooter.area.y + shooter.area.height - ball.r - 2.f;
            ball.vx = 0.f;
            ball.vy = 0.f;
            if (lives <= 0) { lives = 3; score = 0; }
        }
    }
}


void Game::CollideWalls()
{
    // Marco interior
    float leftW = 20.f, topW = 20.f, rightW = (float)W - 20.f, botW = (float)H - 20.f;

    if (ball.tx - ball.r < leftW) {
        ball.tx = leftW + ball.r;
        ball.vx = -ball.vx * wallRest;
    }
    if (ball.tx + ball.r > rightW) {
        ball.tx = rightW - ball.r;
        ball.vx = -ball.vx * wallRest;
    }
    if (ball.ty - ball.r < topW) {
        ball.ty = topW + ball.r;
        ball.vy = -ball.vy * wallRest;
    }
    if (ball.ty + ball.r > botW) {
        ball.ty = botW - ball.r;
        ball.vy = -std::fabs(ball.vy) * wallRest; // hacia arriba
    }
}

void Game::CollideFlippers()
{
    CollisionInfo info;
    if (left.CheckCollision(ball, info))  left.OnCollision(ball, info);
    if (right.CheckCollision(ball, info)) right.OnCollision(ball, info);
}

void Game::CollideTargets()
{
    for (auto& t : targets) {
        CollisionInfo info;
        if (t.CheckCollision(ball, info)) {
            t.OnCollision(ball, info);
            score += t.score;
        }
    }
}

void Game::Draw()
{
    // Fondo
    ClearBackground(Color{ 15,15,20,255 });

    // Marco tablero
    DrawRectangleLines(20, 20, W - 40, H - 40, RAYWHITE);

    // UI
    DrawText(TextFormat("Score: %d", score), 30, 30, 20, RAYWHITE);
    DrawText(TextFormat("Lives: %d", lives), 30, 55, 20, RAYWHITE);
    DrawText("LEFT/RIGHT or A/D = Flippers   SPACE=Charge/Launch   R=Reset Ball", 30, 80, 18, Color{ 180,180,220,255 });

    // Dibujos
    for (auto& t : targets) t.Draw();
    shooter.Draw();
    out.Draw();
    left.Draw();
    right.Draw();
    ball.Draw();
}
