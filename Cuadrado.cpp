#include "Cuadrado.h"

void Cuadrado::FillDDA(int X1, int Y1, int X2, int Y2)
{
    ClearPoints();

    // Guardar puntos del borde
    for (int x = (X1 < X2 ? X1 : X2); x <= (X1 > X2 ? X1 : X2); ++x)
    {
        SavePoint(x, (Y1 < Y2 ? Y1 : Y2));
        SavePoint(x, (Y1 > Y2 ? Y1 : Y2));
    }
    for (int y = (Y1 < Y2 ? Y1 : Y2); y <= (Y1 > Y2 ? Y1 : Y2); ++y)
    {
        SavePoint((X1 < X2 ? X1 : X2), y);
        SavePoint((X1 > X2 ? X1 : X2), y);
    }

    // Relleno y contorno
    FillScanlineY(GREEN);
    DDASquare(X1, Y1, X2, Y2);
}

void Cuadrado::FillBRH(int X1, int Y1, int X2, int Y2)
{
    ClearPoints();

    for (int x = (X1 < X2 ? X1 : X2); x <= (X1 > X2 ? X1 : X2); ++x)
    {
        SavePoint(x, (Y1 < Y2 ? Y1 : Y2));
        SavePoint(x, (Y1 > Y2 ? Y1 : Y2));
    }
    for (int y = (Y1 < Y2 ? Y1 : Y2); y <= (Y1 > Y2 ? Y1 : Y2); ++y)
    {
        SavePoint((X1 < X2 ? X1 : X2), y);
        SavePoint((X1 > X2 ? X1 : X2), y);
    }

    FillScanlineY(YELLOW);
    BRHSquare(X1, Y1, X2, Y2);
}

void Cuadrado::DrawDDA(int X1, int Y1, int X2, int Y2)
{
    DDASquare(X1, Y1, X2, Y2);
}

void Cuadrado::DrawBRH(int X1, int Y1, int X2, int Y2)
{
    BRHSquare(X1, Y1, X2, Y2);
}
