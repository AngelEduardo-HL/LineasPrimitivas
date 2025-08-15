#include "Cuadrado.h"

void Cuadrado::FillDDA(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2)
{
    // Transformar esquinas
    int x1, y1, x2, y2;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);

    ClearPoints();

	// Guardar puntos del borde trasladados
    int Xmin = (x1 < x2) ? x1 : x2;
    int Xmax = (x1 > x2) ? x1 : x2;
    int Ymin = (y1 < y2) ? y1 : y2;
    int Ymax = (y1 > y2) ? y1 : y2;

    for (int x = Xmin; x <= Xmax; ++x) {
        SavePoint(x, Ymin);
        SavePoint(x, Ymax);
    }
    for (int y = Ymin; y <= Ymax; ++y) {
        SavePoint(Xmin, y);
        SavePoint(Xmax, y);
    }

    // Relleno y contorno en la posici�n trasladada
    FillScanlineY(GREEN);
    DDASquare(x1, y1, x2, y2);
}

void Cuadrado::FillBRH(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2)
{
    int x1, y1, x2, y2;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);

    ClearPoints();

    int Xmin = (x1 < x2) ? x1 : x2;
    int Xmax = (x1 > x2) ? x1 : x2;
    int Ymin = (y1 < y2) ? y1 : y2;
    int Ymax = (y1 > y2) ? y1 : y2;

    for (int x = Xmin; x <= Xmax; ++x) {
        SavePoint(x, Ymin);
        SavePoint(x, Ymax);
    }
    for (int y = Ymin; y <= Ymax; ++y) {
        SavePoint(Xmin, y);
        SavePoint(Xmax, y);
    }

    FillScanlineY(YELLOW);
    BRHSquare(x1, y1, x2, y2);
}

void Cuadrado::DrawDDA(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2)
{
    int x1, y1, x2, y2;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);
    DDASquare(x1, y1, x2, y2);
}

void Cuadrado::DrawBRH(const Geometry::Mat3& T, int X1, int Y1, int X2, int Y2)
{
    int x1, y1, x2, y2;
    TransformPoint(T, X1, Y1, x1, y1);
    TransformPoint(T, X2, Y2, x2, y2);
    BRHSquare(x1, y1, x2, y2);
}
