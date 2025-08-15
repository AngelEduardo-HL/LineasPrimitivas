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

void Triangulo::FillDDA(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    int x1, y1, x2, y2, x3, y3;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);
    TransformPoint(T, X3, Y3, x3, y3);

    ClearPoints();
    SaveTriangleEdgesPoints(*this, x1, y1, x2, y2, x3, y3);

    FillScanlineY(GREEN);
    DDATriangle(x1, y1, x2, y2, x3, y3);
}

void Triangulo::FillBRH(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	int x1, y1, x2, y2, x3, y3;
	TransformPoint(T, X1, Y1, x1, y1);
	TransformPoint(T, X2, Y2, x2, y2);
	TransformPoint(T, X3, Y3, x3, y3);

    ClearPoints();
    SaveTriangleEdgesPoints(*this, x1, y1, x2, y2, x3, y3);

    FillScanlineY(YELLOW);
    BRHTriangle(x1, y1, x2, y2, x3, y3);
}

void Triangulo::DrawDDA(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    int x1, y1, x2, y2, x3, y3;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);
    TransformPoint(T, X3, Y3, x3, y3);
    DDATriangle(x1, y1, x2, y2, x3, y3);
}

void Triangulo::DrawBRH(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    int x1, y1, x2, y2, x3, y3;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);
    TransformPoint(T, X3, Y3, x3, y3);
    BRHTriangle(x1, y1, x2, y2, x3, y3);
}
