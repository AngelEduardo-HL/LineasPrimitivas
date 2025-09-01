#include "Game.h"
#include "raylib.h"
#include <cmath>

Game::Game(int w, int h) : W(w), H(h)
{
    // Shooter a la derecha
    shooter.area = { (float)(W - 40), 120.f, 20.f, 600.f };

    // Bola: más alta en el canal
    ball.r = 6.f;
    ball.tx = shooter.area.x + shooter.area.width * 0.5f;
    const float spawnMarginTop = 90.f;
    ball.ty = shooter.area.y + spawnMarginTop + ball.r;
    ball.vx = 0.f; ball.vy = 0.f;

    // Flippers "como antes": arriba en reposo, bajan al presionar
    left.px = 250.f; left.py = 690.f; left.length = 150.f; left.width = 20.f;
    left.restAngle = -25.f; left.hitAngle = 30.f; left.rotDeg = left.restAngle; left.color = Color{ 40,140,255,255 };

    right.px = 550.f; right.py = 690.f; right.length = -150.f; right.width = 20.f;
    right.restAngle = 25.f; right.hitAngle = -30.f; right.rotDeg = right.restAngle; right.color = Color{ 40,140,255,255 };

    // Targets puntuables (arriba) con RELLENO de color
    targets.clear();
    { Target t; t.tx = 400; t.ty = 210; t.r = 22; t.restitution = 1.10f; t.score = 100; t.ring = WHITE; t.filled = true; t.fill = RED;    targets.push_back(t); }   // rojo
    { Target t; t.tx = 310; t.ty = 270; t.r = 22; t.restitution = 1.05f; t.score = 50; t.ring = WHITE; t.filled = true; t.fill = YELLOW; targets.push_back(t); } // amarillo
    { Target t; t.tx = 490; t.ty = 270; t.r = 22; t.restitution = 1.05f; t.score = 25; t.ring = WHITE; t.filled = true; t.fill = GREEN;  targets.push_back(t); } // verde

    // Bumpers grises (más chicos) SIN relleno
    auto addBumper = [&](float x, float y, float r) {
        Target t; t.tx = x; t.ty = y; t.r = r; t.restitution = 1.25f; t.score = 0; t.ring = GRAY; t.filled = false; targets.push_back(t);
        };
    addBumper(150, 180, 14);
    addBumper(650, 180, 14);
    addBumper(400, 360, 14);
    addBumper(245, 540, 12);
    addBumper(555, 540, 12);
    addBumper(650, 320, 12);
    addBumper(150, 320, 12);

    // Triángulos (contorno amarillo vía BRH)
    tris.clear();
	tris.push_back({ 320,500,  360,420,  380,520, 0.75f }); // izq
	tris.push_back({ 480,500,  420,420,  400,520, 0.75f }); // der
	tris.push_back({ 190,640,  260,600,  220,700, 0.75f }); // izq abajo
	tris.push_back({ 610,640,  540,600,  580,700, 0.75f }); // der abajo

    // OutHole (rojo, centrado abajo)
    out.area = { (float)(W / 2 - 60), (float)(H - 22), 120.f, 20.f };
}

void Game::HandleInput()
{
    left.pressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    right.pressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    if (IsKeyPressed(KEY_R)) {
        ball.tx = shooter.area.x + shooter.area.width * 0.5f;
        const float spawnMarginTop = 90.f;
        ball.ty = shooter.area.y + spawnMarginTop + ball.r;
        ball.vx = ball.vy = 0.f;
    }
}

void Game::Update(float dt)
{
    HandleInput();

    left.Update(dt);
    right.Update(dt);

    // shooter antes
    shooter.Update(dt);
    shooter.ApplyToBall(ball);

    // física bola
    ball.vy += gravity * dt;
    ball.tx += ball.vx * dt;
    ball.ty += ball.vy * dt;

    // shooter otra vez (por si en el paso se acerca al pistón)
    shooter.ApplyToBall(ball);

    // colisiones
    CollideBorders();
    CollideFlippers();
    CollideTargets();
    CollideTriangles();

    // fricción
    ball.vx *= friction;
    ball.vy *= friction;

    // OutHole -> respawn
    if (out.Check(ball)) {
        lives--;
        ball.tx = shooter.area.x + shooter.area.width * 0.5f;
        const float spawnMarginTop = 90.f;
        ball.ty = shooter.area.y + spawnMarginTop + ball.r;
        ball.vx = ball.vy = 0.f;
        if (lives <= 0) { lives = 3; score = 0; }
    }
}

void Game::CollideBorders()
{
    // Área jugable sin el canal del shooter
    float left = 20.f;
    float right = shooter.area.x - 2.f;    // pared a la izquierda del canal
    float top = 20.f;
    float bottom = (float)H - 22.f;         // top del OutHole

    // Izquierda
    if (ball.tx - ball.r < left) {
        ball.tx = left + ball.r;
        ball.vx = std::abs(ball.vx);
    }
    // Derecha
    if (ball.tx + ball.r > right) {
        ball.tx = right - ball.r;
        ball.vx = -std::abs(ball.vx);
    }
    // Techo
    if (ball.ty - ball.r < top) {
        ball.ty = top + ball.r;
        ball.vy = std::abs(ball.vy);
    }
    // Suelo: rebota salvo la ventana del OutHole
    if (ball.ty + ball.r > bottom) {
        if (ball.tx < out.area.x || ball.tx > out.area.x + out.area.width) {
            ball.ty = bottom - ball.r;
            ball.vy = -std::abs(ball.vy) * 0.85f;
        }
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
            if (t.score > 0) score += t.score;
        }
    }
}

void Game::CollideTriangles()
{
    auto Clamp = [](float v, float lo, float hi) { return (v < lo) ? lo : ((v > hi) ? hi : v); };
    auto Len = [](float x, float y) { return std::sqrtf(x * x + y * y); };
    auto DistPointSeg = [&](float px, float py, float ax, float ay, float bx, float by, float& qx, float& qy) {
        float vx = bx - ax, vy = by - ay;
        float wx = px - ax, wy = py - ay;
        float vv = vx * vx + vy * vy;
        float t = (vv > 0) ? (wx * vx + wy * vy) / vv : 0.f;
        t = Clamp(t, 0.f, 1.f);
        qx = ax + t * vx; qy = ay + t * vy;
        float dx = px - qx, dy = py - qy;
        return std::sqrtf(dx * dx + dy * dy);
        };

    for (auto& td : tris) {
        int X[3] = { td.x1, td.x2, td.x3 };
        int Y[3] = { td.y1, td.y2, td.y3 };
        for (int e = 0; e < 3; ++e) {
            int i = e, j = (e + 1) % 3;
            float qx, qy;
            float d = DistPointSeg(ball.tx, ball.ty, (float)X[i], (float)Y[i], (float)X[j], (float)Y[j], qx, qy);
            if (d < ball.r) {
                float nx = ball.tx - qx, ny = ball.ty - qy;
                float nlen = Len(nx, ny); if (nlen < 1e-4f) continue;
                nx /= nlen; ny /= nlen;
                float pen = ball.r - d;
                ball.tx += nx * pen; ball.ty += ny * pen;
                float vn = ball.vx * nx + ball.vy * ny;
                ball.vx = ball.vx - (1.0f + td.restitution) * vn * nx;
                ball.vy = ball.vy - (1.0f + td.restitution) * vn * ny;
            }
        }
    }
}

void Game::Draw()
{
    // fondo morado
    ClearBackground(Color{ 120,60,140,255 });

    // contorno del campo (sin canal del shooter)
    DrawLine(20, 20, (int)shooter.area.x - 2, 20, BLACK);
    DrawLine(20, 20, 20, H - 22, BLACK);
    DrawLine((int)shooter.area.x - 2, 20, (int)shooter.area.x - 2, H - 22, BLACK);

    // shooter y out hole
    shooter.Draw();
    out.Draw();

    // targets / bumpers
    for (auto& t : targets) t.Draw();

    // triángulos (BRH amarillo)
    for (auto& td : tris) td.Draw(tr);

    // flippers con color
    left.Draw();
    right.Draw();

    // bola
    ball.Draw();

    // UI
    DrawText(TextFormat("Score: %d", score), 30, 30, 22, RAYWHITE);
    DrawText(TextFormat("Lives: %d", lives), 30, 58, 22, RAYWHITE);
    DrawText("LEFT/RIGHT or A/D = Flippers   SPACE = Spring   R = Reset Ball", 30, 84, 18, Color{ 180,180,220,255 });
}
