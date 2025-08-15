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

void Circulo::FillDDA(const Geometry::Mat3& T, int cx, int cy, float r)
{
	// Transformar centro y radio
	int x, y;
	TransformPoint(T, cx, cy, x, y);
    SaveCirclePerimeterPoints(*this, x, y, r);
    DDACircle((float)x, (float)y, r);
    CircleScanlineY(x, y, (int)(r + 0.5f), GREEN);
}

void Circulo::FillBRH(const Geometry::Mat3& T, int cx, int cy, float r)
{
	// Transformar centro y radio
	int x, y;
	TransformPoint(T, cx, cy, x, y);
    SaveCirclePerimeterPoints(*this, x, y, r);
    BRHCircle(x, y, r);
    CircleScanlineY(x, y, (int)(r + 0.5f), YELLOW);
}

void Circulo::DrawDDA(const Geometry::Mat3& T, int cx, int cy, float r)
{
	// Transformar centro y radio
	int x, y;
	TransformPoint(T, cx, cy, x, y);
    DDACircle((float)x, (float)y, r);
}

void Circulo::DrawBRH(const Geometry::Mat3& T, int cx, int cy, float r)
{
	// Transformar centro y radio
	int x, y;
	TransformPoint(T, cx, cy, x, y);
    BRHCircle(x, y, r);
}
