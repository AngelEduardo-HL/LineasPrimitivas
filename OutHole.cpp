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
    DrawRectangle((int)area.x, (int)area.y, (int)area.width, (int)area.height, RED);
}
