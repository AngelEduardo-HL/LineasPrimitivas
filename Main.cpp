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

	// Transformaciones
	Geometry::Mat3 TDDA = Geometry::Traslacion(100.f, 100.f); //Coordenadas a las que se Traslada
	Geometry::Mat3 TBRH = Geometry::Traslacion(-100.f, 100.f); //Coordenadas a las que se Traslada

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

		//---- Coordenadas orginales ----

        // Casita con DDA
        square.DrawDDA(TDDA, screenWidth / 2, screenHeight / 2, 600, 600);
        triangle.DrawDDA(TDDA, screenWidth / 2, screenHeight / 2, screenWidth / 2 + 200, screenHeight / 2, screenWidth / 2 + 100, screenHeight / 2 - 150);
        circle.DrawDDA(TDDA, 500, 700, 30);

        // Casita con BRH
        square.DrawBRH(TBRH, screenWidth / 2, screenHeight / 2, 200, 200);
        triangle.DrawBRH(TBRH, 400, 200, 300, 50, 200, 200);
        circle.DrawBRH(TBRH, 300, 500, 30);

        // Coloreado
        square.FillBRH(TBRH, screenWidth / 2, screenHeight / 2, 200, 200);
        triangle.FillBRH(TBRH, 400, 200, 300, 50, 200, 200);
        circle.FillBRH(TBRH, 300, 500, 30.f);

        square.FillDDA(TDDA,screenWidth / 2, screenHeight / 2, 600, 600);
        triangle.FillDDA(TDDA, screenWidth / 2, screenHeight / 2, screenWidth / 2 + 200, screenHeight / 2, screenWidth / 2 + 100, screenHeight / 2 - 150);
        circle.FillDDA(TDDA, 500, 700, 30.f);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
