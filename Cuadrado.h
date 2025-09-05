#pragma once
#include "Geometry.h"

class Cuadrado : public Geometry
{
public:
    // Guarda borde y colorea con rasterizacion Y (DDA)
    void FillDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, Color col);

    // Guarda borde y colorea con rasterizacion Y (BRH)
    void FillBRH(const Geometry::Mat3& M,int X1, int Y1, int X2, int Y2, Color col);

    // Solo contorno
    void DrawDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2);
    void DrawBRH(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2);
};
