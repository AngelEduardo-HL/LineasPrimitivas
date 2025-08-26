#include <iostream>
#include "raylib.h"

#include "Game.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "GigaChadWindow - Pinball");
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
