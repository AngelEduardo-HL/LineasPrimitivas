#include "Triangulo.h"

// Guarda los puntos de los bordes del triángulo y los guarda en la geometría
static void SaveTriangleEdgesPoints(Geometry & g, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    auto saveLine = [&](int Ax, int Ay, int Bx, int By)
        {
            float dx = Bx - Ax, dy = By - Ay;
            float adx = (dx < 0) ? -dx : dx, ady = (dy < 0) ? -dy : dy;
            int steps = (int)((adx > ady) ? adx : ady);
            if (steps == 0) { g.SavePoint(Ax, Ay); return; }
            float xInc = dx / steps, yInc = dy / steps;
            float x = (float)Ax, y = (float)Ay;
            for (int i = 0; i <= steps; ++i)
            {
                g.SavePoint((int)(x + 0.5f), (int)(y + 0.5f));
                x += xInc; y += yInc;
            }
        };

    saveLine(X1, Y1, X2, Y2);
    saveLine(X2, Y2, X3, Y3);
    saveLine(X3, Y3, X1, Y1);
}

void Triangulo::FillDDA(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    ClearPoints();
    SaveTriangleEdgesPoints(*this, X1, Y1, X2, Y2, X3, Y3);

    FillScanlineY(GREEN);
    DDATriangle(X1, Y1, X2, Y2, X3, Y3);
}

void Triangulo::FillBRH(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    ClearPoints();
    SaveTriangleEdgesPoints(*this, X1, Y1, X2, Y2, X3, Y3);

    FillScanlineY(YELLOW);
    BRHTriangle(X1, Y1, X2, Y2, X3, Y3);
}

void Triangulo::DrawDDA(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    DDATriangle(X1, Y1, X2, Y2, X3, Y3);
}

void Triangulo::DrawBRH(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    BRHTriangle(X1, Y1, X2, Y2, X3, Y3);
}
