#pragma once
#include "Geometry.h"

class Triangulo : public Geometry
{
public:
    void FillDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, int X3, int Y3);
    void FillBRH(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, int X3, int Y3);

    void DrawDDA(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, int X3, int Y3);
    void DrawBRH(const Geometry::Mat3& M, int X1, int Y1, int X2, int Y2, int X3, int Y3);
};
