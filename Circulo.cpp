#include "Circulo.h"

static void SaveCirclePerimeterPoints(Geometry& g, int cx, int cy, float radio)
{
    g.ClearPoints();
    int r = (int)(radio + 0.5f);
    int x = 0, y = r;
    int d = 1 - r;

    auto save8 = [&](int xx, int yy)
        {
            g.SavePoint(cx + xx, cy + yy);
            g.SavePoint(cx - xx, cy + yy);
            g.SavePoint(cx + xx, cy - yy);
            g.SavePoint(cx - xx, cy - yy);
            g.SavePoint(cx + yy, cy + xx);
            g.SavePoint(cx - yy, cy + xx);
            g.SavePoint(cx + yy, cy - xx);
            g.SavePoint(cx - yy, cy - xx);
        };

    while (x <= y)
    {
        save8(x, y);
        if (d < 0) d += 2 * x + 3;
        else { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

void Circulo::FillDDA(int cx, int cy, float r)
{
    SaveCirclePerimeterPoints(*this, cx, cy, r);
    DDACircle((float)cx, (float)cy, r);
    CircleScanlineY(cx, cy, (int)(r + 0.5f), GREEN);
}

void Circulo::FillBRH(int cx, int cy, float r)
{
    SaveCirclePerimeterPoints(*this, cx, cy, r);
    BRHCircle(cx, cy, r);
    CircleScanlineY(cx, cy, (int)(r + 0.5f), YELLOW);
}

void Circulo::DrawDDA(int cx, int cy, float r)
{
    DDACircle((float)cx, (float)cy, r);
}

void Circulo::DrawBRH(int cx, int cy, float r)
{
    BRHCircle(cx, cy, r);
}
