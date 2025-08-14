#pragma once
#include "Geometry.h"

class Triangulo : public Geometry
{
public:
    void FillDDA(int X1, int Y1, int X2, int Y2, int X3, int Y3);
    void FillBRH(int X1, int Y1, int X2, int Y2, int X3, int Y3);

    void DrawDDA(int X1, int Y1, int X2, int Y2, int X3, int Y3);
    void DrawBRH(int X1, int Y1, int X2, int Y2, int X3, int Y3);
};
