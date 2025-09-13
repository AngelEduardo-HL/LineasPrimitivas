#include "OutHole.h"
#include "Ball.h"
#include "raylib.h"

bool OutHole::Check(const Ball& b) const
{
    return (b.tx >= area.x && b.tx <= area.x + area.width &&
        b.ty >= area.y && b.ty <= area.y + area.height);
}

void OutHole::Draw()
{
	// Dibujar el rectángulo del OutHole
	Geometry::Mat3 I = Geometry::Traslacion(0, 0);
	drawer.FillBRH(I, (int)area.x, (int)area.y, (int)(area.x + area.width), (int)(area.y + area.height), RED);
}
