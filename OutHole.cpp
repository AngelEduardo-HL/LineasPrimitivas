#include "OutHole.h"
#include "Ball.h"

bool OutHole::Check(const Ball& b) const
{
    return (b.tx >= x && b.tx <= x + w &&
        b.ty >= y && b.ty <= y + h);
}

void OutHole::Draw()
{
    Geometry::Mat3 I = Geometry::Traslacion(0, 0);

    const int x1 = x;
    const int y1 = y;
    const int x2 = x + w;
    const int y2 = y + h;

    drawer.FillBRH(I, x1, y1, x2, y2, RED);
    drawer.DrawBRH(I, x1, y1, x2, y2);
}
