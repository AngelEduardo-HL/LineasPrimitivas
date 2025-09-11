#include <iostream>
#include "raylib.h"
#include "Game.h"

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Pinball BRH");
    SetTargetFPS(60);

    Game game(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        BeginDrawing();
        game.Update(dt);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
