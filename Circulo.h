#pragma once
#include "Geometry.h"

class Circulo : public Geometry
{
public:
    void FillDDA(const Geometry::Mat3& M, int cx, int cy, float r, Color col);
    void FillBRH(const Geometry::Mat3& M, int cx, int cy, float r, Color col);

    void DrawDDA(const Geometry::Mat3& M, int cx, int cy, float r);
    void DrawBRH(const Geometry::Mat3& M, int cx, int cy, float r);
};
