#include "Cuadrado.h"

// DDA para muestrear un segmento y guardar puntos de borde (sirve al scanline Y)
static void SaveLineDDA(Geometry& g, int X1, int Y1, int X2, int Y2)
{
    float dx = X2 - X1, dy = Y2 - Y1;
    float adx = (dx < 0) ? -dx : dx, ady = (dy < 0) ? -dy : dy;
    int steps = (int)((adx > ady) ? adx : ady);
    if (steps <= 0) { g.SavePoint(X1, Y1); return; }

    float xInc = dx / steps, yInc = dy / steps;
    float x = (float)X1, y = (float)Y1;
    for (int i = 0; i <= steps; ++i)
    {
        g.SavePoint((int)(x + 0.5f), (int)(y + 0.5f));
        x += xInc; y += yInc;
    }
}

void Cuadrado::FillDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, Color col)
{
    //Evita que el relleno desaparezca con la rotacion
	if (X1 == X2 || Y1 == Y2) return; // Evita relleno de lineas

    //Transformar las 4 esquinas del rectángulo original
    int ax, ay, bx, by, cx, cy, dx, dy;
    TransformPoint(M, X1, Y1, ax, ay); // A: arriba-izq
    TransformPoint(M, X2, Y1, bx, by); // B: arriba-der
    TransformPoint(M, X2, Y2, cx, cy); // C: abajo-der
    TransformPoint(M, X1, Y2, dx, dy); // D: abajo-izq

    //Guardar bordes rotados/escalados con DDA
    ClearPoints();
    SaveLineDDA(*this, ax, ay, bx, by);
    SaveLineDDA(*this, bx, by, cx, cy);
    SaveLineDDA(*this, cx, cy, dx, dy);
    SaveLineDDA(*this, dx, dy, ax, ay);

    //Rellenar y dibujar contorno
    FillScanlineY(col);
    DDALine(ax, ay, bx, by);
    DDALine(bx, by, cx, cy);
    DDALine(cx, cy, dx, dy);
    DDALine(dx, dy, ax, ay);
}

void Cuadrado::FillBRH(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, Color col)
{
	// Evita que el relleno desaparezca con la rotacion
	if (X1 == X2 || Y1 == Y2) return; // Evita relleno de lineas

	// Transformar las 4 esquinas del rectángulo original
    int ax, ay, bx, by, cx, cy, dx, dy;
    TransformPoint(M, X1, Y1, ax, ay);
    TransformPoint(M, X2, Y1, bx, by);
    TransformPoint(M, X2, Y2, cx, cy);
    TransformPoint(M, X1, Y2, dx, dy);

	// Guardar bordes rotados/escalados con BRH
    ClearPoints();
    SaveLineDDA(*this, ax, ay, bx, by);
    SaveLineDDA(*this, bx, by, cx, cy);
    SaveLineDDA(*this, cx, cy, dx, dy);
    SaveLineDDA(*this, dx, dy, ax, ay);

	// Rellenar y dibujar contorno
    FillScanlineY(col);
    BRHLine(ax, ay, bx, by);
    BRHLine(bx, by, cx, cy);
    BRHLine(cx, cy, dx, dy);
    BRHLine(dx, dy, ax, ay);
}

void Cuadrado::DrawDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2)
{
    int ax, ay, bx, by, cx, cy, dx, dy;
    TransformPoint(M, X1, Y1, ax, ay);
    TransformPoint(M, X2, Y1, bx, by);
    TransformPoint(M, X2, Y2, cx, cy);
    TransformPoint(M, X1, Y2, dx, dy);

    DDALine(ax, ay, bx, by);
    DDALine(bx, by, cx, cy);
    DDALine(cx, cy, dx, dy);
    DDALine(dx, dy, ax, ay);
}

void Cuadrado::DrawBRH(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2)
{
    int ax, ay, bx, by, cx, cy, dx, dy;
    TransformPoint(M, X1, Y1, ax, ay);
    TransformPoint(M, X2, Y1, bx, by);
    TransformPoint(M, X2, Y2, cx, cy);
    TransformPoint(M, X1, Y2, dx, dy);

    BRHLine(ax, ay, bx, by);
    BRHLine(bx, by, cx, cy);
    BRHLine(cx, cy, dx, dy);
    BRHLine(dx, dy, ax, ay);
}
