#pragma once
#include "Geometry.h"

class Cuadrado : public Geometry
{
public:
    // Guarda borde y colorea con rasterizacion Y (DDA)
    void FillDDA(int X1, int Y1, int X2, int Y2);

    // Guarda borde y colorea con rasterizacion Y (BRH)
    void FillBRH(int X1, int Y1, int X2, int Y2);

    // Solo contorno
    void DrawDDA(int X1, int Y1, int X2, int Y2);
    void DrawBRH(int X1, int Y1, int X2, int Y2);
};
