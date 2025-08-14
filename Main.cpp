#include <iostream>
#include "raylib.h"

#include "Geometry.h"
#include "Cuadrado.h"
#include "Triangulo.h"
#include "Circulo.h"


// ======================= Main =======================
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "GigaChadWindow");
    SetTargetFPS(60);

	// Inicializar geometría
	Cuadrado square;
	Triangulo triangle;
	Circulo circle;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Casita con DDA
        square.DrawDDA(screenWidth / 2, screenHeight / 2, 600, 600);
        triangle.DrawDDA(screenWidth / 2, screenHeight / 2, screenWidth / 2 + 200, screenHeight / 2, screenWidth / 2 + 100, screenHeight / 2 - 150);
        circle.DrawDDA(500, 700, 30);

        // Casita con BRH
        square.DrawBRH(screenWidth / 2, screenHeight / 2, 200, 200);
        triangle.DrawBRH(400, 200, 300, 50, 200, 200);
        circle.DrawBRH(300, 500, 30);

        // Coloreado
        square.FillBRH(screenWidth / 2, screenHeight / 2, 200, 200);
        triangle.FillBRH(400, 200, 300, 50, 200, 200);
        circle.FillBRH(300, 500, 30.f);

        square.FillDDA(screenWidth / 2, screenHeight / 2, 600, 600);
        triangle.FillDDA(screenWidth / 2, screenHeight / 2, screenWidth / 2 + 200, screenHeight / 2, screenWidth / 2 + 100, screenHeight / 2 - 150);
        circle.FillDDA(500, 700, 30.f);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
