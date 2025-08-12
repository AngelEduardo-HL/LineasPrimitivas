#pragma once
#include <iostream>
#include "raylib.h"
//Es la clase padre de la que heredan las figuras (Heredan las funciones de dibujo, guardado de puntos y matrices de translacion, rotacion y escalamiento)

class Geometry
{
    public:

    struct Point { int x, y; };

    #define MAX_POINTS 2000
    static Point pts[MAX_POINTS];
    static int ptsCount;

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

    //Tipos de lineas
    //Linea DDA es la Base para las figuras con DDA
    void DDALine(int X1, int Y1, int X2, int Y2, Color col)
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

    void BRHLine(int X1, int Y1, int X2, int Y2, Color col)
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

	//Matrices de Translacion, Rotacion y Escalacion
    // 
    //Matris de translacion
    void Translate(int x, int y)
    {
        for (int i = 0; i < ptsCount; i++)
        {
            pts[i].x += x;
            pts[i].y += y;
        }
    }

	//Matris de rotacion
	void Rotate(float angle)
	{
		float rad = angle * PI / 180.0f; // Convertir a radianes
		for (int i = 0; i < ptsCount; i++)
		{
			int x = pts[i].x;
			int y = pts[i].y;
			pts[i].x = (int)(x * cos(rad) - y * sin(rad));
			pts[i].y = (int)(x * sin(rad) + y * cos(rad));
		}
	}
};

