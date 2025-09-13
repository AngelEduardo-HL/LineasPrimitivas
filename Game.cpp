#include "Game.h"
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

bool ShooterGuide::CollideAndBounce(Ball& b) const
{
    float vx = bx - ax, vy = by - ay;
    float wx = b.tx - ax, wy = b.ty - ay;
    float vv = vx * vx + vy * vy;
    float t = (vv > 0) ? (wx * vx + wy * vy) / vv : 0.f;
    t = Clamp(t, 0.f, 1.f);
    float qx = ax + t * vx, qy = ay + t * vy;

    float dx = b.tx - qx, dy = b.ty - qy;
    float d = std::sqrtf(dx * dx + dy * dy);
    float rad = b.r + thick * 0.5f;


    if (d < rad) {
        float nx = (d > 1e-6f) ? dx / d : 0.f;
        float ny = (d > 1e-6f) ? dy / d : -1.f;
        float pen = rad - d;

        b.tx += nx * pen; b.ty += ny * pen;

        float vn = b.vx * nx + b.vy * ny;
        b.vx = b.vx - (1.0f + restitution) * vn * nx;
        b.vy = b.vy - (1.0f + restitution) * vn * ny;
        return true;
    }
    return false;
}

Game::Game(int w, int h) : W(w), H(h)
{
    // Shooter a la derecha
    shooter.area = { (float)(W - 40), 120.f, 20.f, 600.f };

    // Bola: más alta en el canal
    ball.r = 5.0f;
    ball.tx = shooter.area.x + shooter.area.width * 0.5f;
    const float spawnMarginTop = 90.f;
    ball.ty = shooter.area.y + spawnMarginTop + ball.r;
    ball.vx = 0.f; ball.vy = 0.f;

	// Flippers izquierdo y derecho
	left.px = 175.f; left.py = 670.f; left.length = 120.f; left.width = 20.f;// pivote en extremo derecho
    left.restAngle = 30.f; left.hitAngle = -25.f; left.rotDeg = left.restAngle; left.color = DARKBLUE;

    right.px = 420.f; right.py = 670.f; right.length = -120.f; right.width = 20.f;
    right.restAngle = -30.f; right.hitAngle = 25.f; right.rotDeg = right.restAngle; right.color = DARKBLUE;

    // Targets superiores con relleno
    targets.clear();
    // (antes: 300,195 | 225,260 | 375,260)
    { Target t; t.tx = 300; t.ty = 325; t.r = 6; t.restitution = 0.8f; t.score = 100; t.ring = GRAY;  t.filled = true; t.fill = RED;    targets.push_back(t); }
    { Target t; t.tx = 185; t.ty = 390; t.r = 6; t.restitution = 0.8f; t.score = 50;  t.ring = GRAY;  t.filled = true; t.fill = YELLOW; targets.push_back(t); }
    { Target t; t.tx = 400; t.ty = 390; t.r = 6; t.restitution = 0.8f; t.score = 25;  t.ring = GRAY;  t.filled = true; t.fill = GREEN;  targets.push_back(t); }
    { Target t; t.tx = 300; t.ty = 65; t.r = 6; t.restitution = 0.8f; t.score = 100;  t.ring = GRAY;  t.filled = true; t.fill = RED;  targets.push_back(t); }
    { Target t; t.tx = 300; t.ty = 520; t.r = 6; t.restitution = 0.8f; t.score = 50;  t.ring = GRAY;  t.filled = true; t.fill = YELLOW;  targets.push_back(t); }
    { Target t; t.tx = 75; t.ty = 585; t.r = 6; t.restitution = 0.8f; t.score = 25;  t.ring = GRAY;  t.filled = true; t.fill = GREEN;  targets.push_back(t); }
    { Target t; t.tx = 490; t.ty = 585; t.r = 6; t.restitution = 0.8f; t.score = 25;  t.ring = GRAY;  t.filled = true; t.fill = GREEN;  targets.push_back(t); }


    // Bumpers grises pequeños
    auto addBumper = [&](float x, float y, float r) {
		// BUMPER gris
        Target t; t.tx = x; t.ty = y; t.r = r; t.restitution = 0.9f; t.score = 0; t.ring = GRAY; t.filled = true; targets.push_back(t);
        };
    addBumper(150, 130, 4);
    //addBumper(300, 65, 4);
    addBumper(450, 130, 4);

    addBumper(300, 195, 4);
    addBumper(225, 260, 4);
    addBumper(375, 260, 4);

    //addBumper(300, 520, 4);
	//addBumper(490, 585, 4);
	//addBumper(75, 585, 4);


    // Triángulos (solo contorno BRH)
    tris.clear();
	tris.push_back({ 150,520,  225,550,  150,455, 1.2f });// coords (x1,y1, x2,y2, x3,y3, Rebote)
    tris.push_back({ 475,532,  400,562,  475,466,  1.2f });

    // OutHole
    out.x = W / 2 - 60;
    out.y = H - 22;
    out.w = 120;
    out.h = 20;

    // Guía inclinada para salir del shooter
	// coordenadas del segmento
   /* guide.ax = shooter.area.x - 6.f;
    guide.ay = shooter.area.y + 32.f;
    guide.bx = shooter.area.x - 54.f;
    guide.by = shooter.area.y + 84.f;
    guide.thick = 14.f;
    guide.restitution = 0.95f;*/

	// Bordes del campo
	walls.clear();
    // === Embudo hacia el OutHole ===
    // izquierda
    walls.push_back(Wall::FromAB(
        10.f, 680.f,
        (float)out.Left() - 8.f, (float)out.Bottom(),
        14.f, BLACK, 0.98f));

	// derecha
    walls.push_back(Wall::FromAB(
        550.f, 680.f,
        (float)out.Right() + 8.f, (float)out.Bottom(),
        14.f, BLACK, 0.98f));

    // === Forma de reloj ===
    walls.push_back(Wall::FromAB(
		20.f, 170.f,
		130.f, 320.f,
        12.f, BLACK, 0.98f));
    walls.push_back(Wall::FromAB(
        130.f, 320.f,
        20.f, 455.f,
        12.f, BLACK, 0.98f));
    walls.push_back(Wall::FromAB(
        shooter.area.x - 20.f, 170.f,
        shooter.area.x - 130.f, 320.f,
        12.f, BLACK, 0.98f));
    walls.push_back(Wall::FromAB(
        shooter.area.x - 130.f, 320.f,
        shooter.area.x - 20.f, 455.f,
        12.f, BLACK, 0.98f));

	//=== Bordes de la Pantalla ===
	walls.push_back(Wall::FromAB(20.f, 20.f, 20.f, (float)(H - 22), 14.f, BLACK, 0.98f)); // izquierda
	walls.push_back(Wall::FromAB(20.f, 20.f, (float)(shooter.area.x - 2), 20.f, 14.f, BLACK, 0.98f)); // arriba
	walls.push_back(Wall::FromAB((float)(shooter.area.x - 2), 180.f, (float)(shooter.area.x - 2), (float)(H - 22), 14.f, BLACK, 0.98f)); // derecha del canal

    // barrita superior
	tr = Triangulo();
}

void Game::HandleInput()
{
	// Controles globales
    if (state != GameState::PLAYING) {
		//Nueva partida
        if (IsKeyPressed(KEY_ENTER)) {
            score = 0;
            lives = 3;
            state = GameState::PLAYING;
            ResetBallInShooter();
        }
        return;
    }

    // Controles de flippers
    left.pressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    right.pressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);

    // Reset manual de la bola -> menos una vida
    if (IsKeyPressed(KEY_R)) {
        lives--;
        if (lives <= 0) {
            state = GameState::LOST;
        }
        else {
            ResetBallInShooter();
        }
    }
}

void Game::Update(float dt)
{
    HandleInput();

	if (state != GameState::PLAYING) return;

    // mover flippers
    left.Update(dt);
    right.Update(dt);

	// no perder la bola
    float maxSpeed = std::max(std::fabs(ball.vx), std::fabs(ball.vy));
    float maxMove = maxSpeed * dt;
    int steps = (int)std::ceilf(maxMove / (ball.r * 0.5f));
    if (steps < 1) steps = 1;
    if (steps > 16) steps = 16;
    float h = dt / steps;

    for (int i = 0; i < steps; ++i)
    {
        shooter.Update(h);
        shooter.ApplyToBall(ball);

        // Física
        ball.vy += gravity * h;
        ball.tx += ball.vx * h;
        ball.ty += ball.vy * h;

        // Colisiones
        CollideBorders();

        for (auto& w : walls) w.CollideAndBounce(ball);

        CollideFlippers();
        CollideTargets();
        CollideTriangles();
    }

    // fricción
    ball.vx *= friction;
    ball.vy *= friction;

    if (score >= winScore) 
    {
        state = GameState::WON;
    }

	// OutHole -> respawn y pierde vida o fin de juego
    if (out.Check(ball)) {
        lives--;
        if (lives <= 0) {
            state = GameState::LOST;
        }
        else {
            ResetBallInShooter();
        }
    }
}

void Game::CollideBorders()
{
	if (shooter.ContainsBall(ball)) return; // dentro del canal, no colisiona

    float leftB = 20.f;
    float rightB = shooter.area.x - 2.f; 
    float topB = 20.f;
    float botB = (float)H - 22.f; 

    if (ball.tx - ball.r < leftB) { ball.tx = leftB + ball.r; ball.vx = std::abs(ball.vx); }
    if (ball.tx + ball.r > rightB) { ball.tx = rightB - ball.r; ball.vx = -std::abs(ball.vx); }
    if (ball.ty - ball.r < topB) { ball.ty = topB + ball.r; ball.vy = std::abs(ball.vy); }
    if (ball.ty + ball.r > botB) {
        if (ball.tx < out.Left() || ball.tx > out.Right()) {
            ball.ty = botB - ball.r;
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

void Game::ResetBallInShooter()
{
    ball.tx = shooter.area.x + shooter.area.width * 0.5f;
    const float spawnMarginTop = 90.f;
    ball.ty = shooter.area.y + spawnMarginTop + ball.r;
    ball.vx = 0.f;
    ball.vy = 0.f;

	//Reposo del shooter
    shooter.comp = 0.f;
    shooter.prevComp = 0.f;
    shooter.releasing = false;
}

void Game::Draw()
{
    ClearBackground(Color{ 120,60,140,255 });

	// Bordes del campo y canal del shooter
    Geometry g;
	g.BRHLine(20, 20, (int)shooter.area.x - 2, 20, BLACK); // arriba
	g.BRHLine(20, 20, 20, H - 22, BLACK); // izquierda
	//g.BRHLine((int)shooter.area.x - 2, 20, (int)shooter.area.x - 2, H - 22, BLACK); // derecha del canal

	for (auto& w : walls) w.Draw();

    // Shooter y guía
    shooter.Draw();
    guide.Draw();

    // OutHole
    out.Draw();

    // Targets / bumpers
    for (auto& t : targets) t.Draw();

    // Triángulos (BRH)
    for (auto& td : tris) td.Draw(tr);

    // Flippers
    left.Draw();
    right.Draw();

    // Bola
    ball.Draw();

    // UI
    DrawText(TextFormat("Score: %d", score), 30, 30, 22, RAYWHITE);
    DrawText(TextFormat("Lives: %d", lives), 30, 58, 22, RAYWHITE);
    DrawText("LEFT/RIGHT or A/D = Flippers   SPACE = Spring   R = Reset Ball",
        30, 84, 18, Color{ 180,180,220,255 });

    // Mensajes de estado
    if (state != GameState::PLAYING)
    {
        Cuadrado q;
		Geometry::Mat3 I = Geometry::Traslacion(0, 0);
		q.FillBRH(I, 0, H / 2 - 60, W, H / 2 + 60, Color{ 0,0,0,180 });

        const char* mainText = (state == GameState::WON) ? "GANASTE!!" : "¡PERDISTE!!";
        const char* subText = "Pulsa ENTER para nueva partida";
        int mainSize = 48;
        int subSize = 22;

        int tw = MeasureText(mainText, mainSize);
        DrawText(mainText, (W - tw) / 2, H / 2 - mainSize, mainSize, RAYWHITE);

        int ts = MeasureText(subText, subSize);
        DrawText(subText, (W - ts) / 2, H / 2 + 8, subSize, Color{ 220,220,220,255 });
    }
}
