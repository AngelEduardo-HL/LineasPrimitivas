#include <iostream>
#include "raylib.h"

struct Point { int x, y; };

#define MAX_POINTS 2000
static Point pts[MAX_POINTS];
static int ptsCount = 0;

//Hacer las funciones para guardar los puntos de las figuras
void clearPoints()
{
    ptsCount = 0;
}

//Guardar los puntos de la figura
void SavePoints(int X, int Y)
{
    if (ptsCount < MAX_POINTS)
    {
		pts[ptsCount++] = { X, Y };
    }
}

//Guardar los puntos del cuadrado
void SavePointsSquare(int X1, int Y1, int X2, int Y2)
{
    clearPoints();
	int Xmin = (X1 < X2) ? X1 : X2; // Minimo entre X1 y X2
	int Xmax = (X1 > X2) ? X1 : X2; // Maximo entre X1 y X2
	int Ymin = (Y1 < Y2) ? Y1 : Y2; // Minimo entre Y1 y Y2
	int Ymax = (Y1 > Y2) ? Y1 : Y2; // Maximo entre Y1 y Y2

	// Guarda arriba y abajo
	for (int x = Xmin; x <= Xmax; x++)
	{
		SavePoints(x, Ymin);
		SavePoints(x, Ymax);
	}
	// Izquierda y derecha
	for (int y = Ymin; y <= Ymax; y++)
	{
		SavePoints(Xmin, y);
		SavePoints(Xmax, y);
	}
}

void SavePointsTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    clearPoints();

	int dx1 = X2 - X1, dy1 = Y2 - Y1; // Lado 1
	int dx2 = X3 - X2, dy2 = Y3 - Y2; // Lado 2
	int dx3 = X1 - X3, dy3 = Y1 - Y3; // Lado 3

    // Lado 1: (X1,Y1)->(X2,Y2)
    {
        float dx = X2 - X1, dy = Y2 - Y1;
        float ax = (dx < 0) ? -dx : dx, ay = (dy < 0) ? -dy : dy;
        int steps = (int)((ax > ay) ? ax : ay);
        if (steps == 0) { SavePoints(X1, Y1); }
        else {
            float xInc = dx / steps, yInc = dy / steps;
            float x = (float)X1, y = (float)Y1;
            for (int i = 0; i <= steps; i++) {
                SavePoints((int)(x + 0.5f), (int)(y + 0.5f));
                x += xInc; y += yInc;
            }
        }
    }
    // Lado 2: (X2,Y2)->(X3,Y3)
    {
        float dx = X3 - X2, dy = Y3 - Y2;
        float ax = (dx < 0) ? -dx : dx, ay = (dy < 0) ? -dy : dy;
        int steps = (int)((ax > ay) ? ax : ay);
        if (steps == 0) { SavePoints(X2, Y2); }
        else {
            float xInc = dx / steps, yInc = dy / steps;
            float x = (float)X2, y = (float)Y2;
            for (int i = 0; i <= steps; i++) {
                SavePoints((int)(x + 0.5f), (int)(y + 0.5f));
                x += xInc; y += yInc;
            }
        }
    }
    // Lado 3: (X3,Y3)->(X1,Y1)
    {
        float dx = X1 - X3, dy = Y1 - Y3;
        float ax = (dx < 0) ? -dx : dx, ay = (dy < 0) ? -dy : dy;
        int steps = (int)((ax > ay) ? ax : ay);
        if (steps == 0) { SavePoints(X3, Y3); }
        else {
            float xInc = dx / steps, yInc = dy / steps;
            float x = (float)X3, y = (float)Y3;
            for (int i = 0; i <= steps; i++) {
                SavePoints((int)(x + 0.5f), (int)(y + 0.5f));
                x += xInc; y += yInc;
            }
        }
    }
}

void SavePointsCircle(int cx, int cy, float radio)
{
    clearPoints();
	int r = (int)(radio + 0.5f);
	int x = 0;
	int y = r;
	int d = 1 - r;

	auto save8 = [&](int x, int y) 
	{
		SavePoints(cx + x, cy + y);
		SavePoints(cx - x, cy + y);
		SavePoints(cx + x, cy - y);
		SavePoints(cx - x, cy - y);
		SavePoints(cx + y, cy + x);
		SavePoints(cx - y, cy + x);
		SavePoints(cx + y, cy - x);
		SavePoints(cx - y, cy - x);
	};

	while (x <= y)
	{
		save8(x, y);
		if (d < 0)
		{
			d += 2 * x + 3; // Incremento en el caso de que la desviación sea negativa
		}
		else
		{
			d += 2 * (x - y) + 5; // Incremento en el caso de que la desviación sea positiva
			y--; // Reducir y si la desviación es positiva
		}
		x++;
	}
}



//Linea DDA es la Base para las figuras con DDA
void DDALine(int X1, int Y1, int X2, int Y2, Color col = GREEN)
{
    // 1. Cálculo de diferenciales
    float dx = X2 - X1;
    float dy = Y2 - Y1;
    // 2. Valor absoluto 
    float absdx = (dx < 0) ? -dx : dx;
    float absdy = (dy < 0) ? -dy : dy;
    // 3. Pasos
    int steps = (absdx > absdy) ? (int)absdx : (int)absdy;
    // 4. Incrementos
    float xInc = dx / steps;
    float yInc = dy / steps;
    // 5. Punto inicial
    float x = X1;
    float y = Y1;
    // 6. Recorrido
    for (int i = 0; i <= steps; i++)
    {
        DrawPixel((int)(x + 0.5f), (int)(y + 0.5f), col);
        x += xInc;
        y += yInc;
    }
}

//Dibuja el circulo usando las lineas DDA
void DDALCircleLine(float X, float Y, float radio, float grados)
{

    for (int i = 0; i <= 36; i++)
    {
        float rad = grados * PI / 180;

        float X2 = X + radio * cos(rad);
        float Y2 = Y + radio * sin(rad);


        DDALine(X, Y, X2, Y2);

        grados = grados + 10;


    }
}

//Lineas Bresenham base para las figuras con BRHL
void BRHLine(int X1, int Y1, int X2, int Y2, Color col = YELLOW)
{
    int DX = abs(X2 - X1);
    int StepsX = (X1 < X2) ? 1 : -1;
    int DY = -abs(Y2 - Y1);
    int StepsY = (Y1 < Y2) ? 1 : -1;
    int Desviacion = DX + DY;  // dx - |dy|
    while (true)
    {
        DrawPixel(X1, Y1, col);
        if (X1 == X2 && Y1 == Y2) break;
        int e2 = Desviacion * 2;
        if (e2 >= DY) { Desviacion += DY; X1 += StepsX; }
        if (e2 <= DX) { Desviacion += DX; Y1 += StepsY; }
    }
}

//Hace el circito con las lineas de BRH
void BRHCircleLine(int X1, int Y1, float radio, float Grados)
{
    for (int i = 0; i < 36; i++)
    {
        float rad = Grados * PI / 180.0f;
        // punto final en la circunferencia
        int x2 = X1 + (int)(radio * cos(rad) + 0.5f);
        int y2 = Y1 + (int)(radio * sin(rad) + 0.5f);
        BRHLine(X1, Y1, x2, y2);

        Grados += 10.0f;
    }
}

//Funciones para dibujar Figuras Completas (Cuadrado, Triangulo y Circulo)
//------------------------------------------------------------------------------------

//Dibuja una Triangulo con DDA
void DDATriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	//DDA
	DDALine(X1, Y1, X2, Y2);
	DDALine(X2, Y2, X3, Y3);
	DDALine(X3, Y3, X1, Y1);
}

//Dibuja un cuadrado usando las lineas DDA
void DDASquare(int X1, int Y1, int X2, int Y2)
{
	//DDA
	DDALine(X1, Y1, X2, Y1);
	DDALine(X2, Y1, X2, Y2);
	DDALine(X2, Y2, X1, Y2);
	DDALine(X1, Y2, X1, Y1);
}

//Dibuja el Circulo completo con las lineas DDA
void DDALCircle(float X, float Y, float radio, float grados)
{

    for (int i = 0; i <= 36; i++)
    {
        float rad = grados * PI / 180;

        float X2 = X + radio * cos(rad);
        float Y2 = Y + radio * sin(rad);

        grados = grados + 10;

		rad = grados * PI / 180;

		float X3 = X + radio * cos(rad);
		float Y3 = Y + radio * sin(rad);

		DDALine(X2, Y2, X3, Y3);


    }
}

//------------------------------------------------------------------------------------
//Dibuja Figuras con BRH
//------------------------------------------------------------------------------------

//Dibuja un cuadrado usando las lineas de Bresenham
void BRHsqueare(int X1, int Y1, int X2, int Y2)
{
	//Bresenham
	BRHLine(X1, Y1, X2, Y1);
	BRHLine(X2, Y1, X2, Y2);
	BRHLine(X2, Y2, X1, Y2);
	BRHLine(X1, Y2, X1, Y1);
}

//Dibuja un triangulo usando las lineas de Bresenham
void BRHTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	//Bresenham
	BRHLine(X1, Y1, X2, Y2);
	BRHLine(X2, Y2, X3, Y3);
	BRHLine(X3, Y3, X1, Y1);
}

//Dibuja el Circulo con las lineas de Bresenham

void BRHCircle(int X1, int Y1, float radio, float Grados)
{
    for (int i = 0; i < 36; i++)
    {
        float rad = Grados * PI / 180.0f;
        // punto final en la circunferencia
        int x2 = X1 + (int)(radio * cos(rad) + 0.5f);
        int y2 = Y1 + (int)(radio * sin(rad) + 0.5f);

        Grados += 10.0f;


		//Calcular el siguiente punto en la circunferencia
        rad = Grados * PI / 180.0f;

		//Punto final en la circunferencia
        int x3 = X1 + (int)(radio * cos(rad) + 0.5f);
        int y3 = Y1 + (int)(radio * sin(rad) + 0.5f);
        BRHLine(x2, y2, x3, y3);

    }
}

//------------------------------------------------------------------------------------
//Rellena las figuras con DDA y BRH
//------------------------------------------------------------------------------------

//Rellena la figura guardada con DDA
void FillWithDDA(int cx, int cy)
{
    for (int i = 0; i < ptsCount; i++)
    {
        DDALine(cx, cy, pts[i].x, pts[i].y);
    }
}

//Rellena la figura guardada con BRH
void FillWithBRH(int cx, int cy)
{
	for (int i = 0; i < ptsCount; i++)
	{
		BRHLine(cx, cy, pts[i].x, pts[i].y);
	}
}

//Colorea CuadradoDDA
void FillSquareDDA(int X1, int Y1, int X2, int Y2)
{
	SavePointsSquare(X1, Y1, X2, Y2);
	DDASquare(X1, Y1, X2, Y2);

	int cx = (X1 + X2) / 2;
	int cy = (Y1 + Y2) / 2;

	FillWithDDA(cx, cy);
}

//Colorea TrianguloDDA
void FillTriangleDAA(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	SavePointsTriangle(X1, Y1, X2, Y2, X3, Y3);
	DDATriangle(X1, Y1, X2, Y2, X3, Y3);
	int cx = (X1 + X2 + X3) / 3;
	int cy = (Y1 + Y2 + Y3) / 3;
	FillWithDDA(cx, cy);
}

//Colorea CirculoDDA
void FillCircleDDA(int cx, int cy, float radio)
{
	SavePointsCircle(cx, cy, radio);
	DDALCircle(cx, cy, radio, 0);
	int x = (int)(cx + 0.5f);
	int y = (int)(cy + 0.5f);
	FillWithDDA(x, y);
}

//Colorea CuadradoBRH
void FillSquareBRH(int X1, int Y1, int X2, int Y2)
{
	SavePointsSquare(X1, Y1, X2, Y2);

	BRHsqueare(X1, Y1, X2, Y2);

	int cx = (X1 + X2) / 2;
	int cy = (Y1 + Y2) / 2;

	FillWithBRH(cx, cy);
}

//Colorea TrianguloBRH
void FillTriangleBRH(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	SavePointsTriangle(X1, Y1, X2, Y2, X3, Y3);
	BRHTriangle(X1, Y1, X2, Y2, X3, Y3);
	int cx = (X1 + X2 + X3) / 3;
	int cy = (Y1 + Y2 + Y3) / 3;
	FillWithBRH(cx, cy);
}
//Colorea CirculoBRH
void FillCircleBRH(int cx, int cy, float radio)
{
	SavePointsCircle(cx, cy, radio);
	BRHCircle(cx, cy, radio, 0);
	int x = (int)(cx + 0.5f);
	int y = (int)(cy + 0.5f);
	FillWithBRH(x, y);
}

//------------------------------------------------------------------------------------
//Realiza el movimiento de Translacion, Escala y Rotacion de las figuras
//------------------------------------------------------------------------------------

//Matris de translacion
void Translate(int x, int y)
{
	for (int i = 0; i < ptsCount; i++)
	{
		pts[i].x += x;
		pts[i].y += y;
	}
}

//Aplica la translacion a la figura guardada
void ApplyTranslation(int x, int y)
{
	Translate(x, y);
	for (int i = 0; i < ptsCount; i++)
	{
		DrawPixel(pts[i].x, pts[i].y, RED);
	}
}


//Matris de Escalado

//Matris de Rotacion

//------------------------------------------------------------------------------------
// Main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

		//Casita con DDA
		DDASquare(screenWidth/2, screenHeight/2, 600, 600);
		DDATriangle(screenWidth/2, screenHeight/2, screenWidth/2 + 200, screenHeight/2, screenWidth/2 + 100, screenHeight/2 - 150);
		DDALCircle(500, 700, 30, 0);

        //Casita con BRH
		BRHsqueare(screenWidth / 2, screenHeight / 2, 200, 200);
		BRHTriangle(400, 200, 300, 50, 200, 200);
		BRHCircle(300, 500, 30, 0);

		//Coloreado
		
		FillSquareBRH(screenWidth / 2, screenHeight / 2, 200, 200);
		FillTriangleBRH(400, 200, 300, 50, 200, 200);
		FillCircleBRH(300, 500, 30.f);

        FillSquareDDA(screenWidth / 2, screenHeight / 2, 600, 600);
		FillTriangleDAA(screenWidth / 2, screenHeight / 2, screenWidth / 2 + 200, screenHeight / 2, screenWidth / 2 + 100, screenHeight / 2 - 150);
		FillCircleDDA(500, 700, 30.f);

		//Translacion
		Translate(100, 100);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}