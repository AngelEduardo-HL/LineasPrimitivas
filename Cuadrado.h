#pragma once
#include "Geometry.h"

class Cuadrado : public Geometry
{
public:
    // Guarda borde y colorea con rasterizacion Y (DDA)
    void FillDDA(const Geometry::Mat3& T,const Geometry::Mat3R& R, int X1, int Y1, int X2, int Y2);

    // Guarda borde y colorea con rasterizacion Y (BRH)
    void FillBRH(const Geometry::Mat3& T,const Geometry::Mat3R& R,int X1, int Y1, int X2, int Y2);

    // Solo contorno
    void DrawDDA(const Geometry::Mat3& T,const Geometry::Mat3R& R, int X1, int Y1, int X2, int Y2);
    void DrawBRH(const Geometry::Mat3& T,const Geometry::Mat3R& R, int X1, int Y1, int X2, int Y2);
};
