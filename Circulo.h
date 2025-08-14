#pragma once
#include "Geometry.h"

class Circulo : public Geometry
{
public:
    void FillDDA(int cx, int cy, float r);
    void FillBRH(int cx, int cy, float r);

    void DrawDDA(int cx, int cy, float r);
    void DrawBRH(int cx, int cy, float r);
};
