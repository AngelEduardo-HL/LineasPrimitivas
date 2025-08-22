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


void Circulo::FillDDA(const Geometry::Mat3& M, int cx, int cy, float r)
{
    //Escalamiento del rado
	r *= (M.m[0][0] + M.m[1][1]); // Escala hacia adentro
	r *= (M.m[1][1] + M.m[0][0]); // Escala hacia afuera

	//Rotacion del radio
	r *= sqrtf(M.m[0][0] * M.m[0][0] + M.m[1][0] * M.m[1][0]); // Rotacion del radio
	r *= sqrtf(M.m[0][1] * M.m[0][1] + M.m[1][1] * M.m[1][1]); // Rotacion del radio

	//Transforma, rota y escala el centro y radio
	int x, y;
	TransformPoint(M, cx, cy, x, y);
    SaveCirclePerimeterPoints(*this, x, y, r);
    DDACircle((float)x, (float)y, r);
    CircleScanlineY(x, y, (int)(r + 0.5f), GREEN);
}

void Circulo::FillBRH(const Geometry::Mat3& M, int cx, int cy, float r)
{
    //Escalamiento del rado
    r *= (M.m[0][0] + M.m[1][1]); // Escala hacia adentro
    r *= (M.m[1][1] + M.m[0][0]); // Escala hacia afuera

	//Rotacion del radio
    r *= sqrtf(M.m[0][0] * M.m[0][0] + M.m[1][0] * M.m[1][0]); // Rotacion del radio
    r *= sqrtf(M.m[0][1] * M.m[0][1] + M.m[1][1] * M.m[1][1]); // Rotacion del radio

	// Transformar centro y radio
	int x, y;
	TransformPoint(M, cx, cy, x, y);
    SaveCirclePerimeterPoints(*this, x, y, r);
    BRHCircle(x, y, r);
    CircleScanlineY(x, y, (int)(r + 0.5f), YELLOW);
}

void Circulo::DrawDDA(const Geometry::Mat3& M, int cx, int cy, float r)
{
	// Escalamiento del radio
    r *= (M.m[0][0] + M.m[1][1]); // Escala hacia adentro
    r *= (M.m[1][1] + M.m[0][0]); // Escala hacia afuera

	// Rotacion del radio
    r *= sqrtf(M.m[0][0] * M.m[0][0] + M.m[1][0] * M.m[1][0]); // Rotacion del radio
    r *= sqrtf(M.m[0][1] * M.m[0][1] + M.m[1][1] * M.m[1][1]); // Rotacion del radio

	// Transformar centro y radio
	int x, y;
	TransformPoint(M, cx, cy, x, y);
    DDACircle((float)x, (float)y, r);
}

void Circulo::DrawBRH(const Geometry::Mat3& M, int cx, int cy, float r)
{
    //Escalamiento del rado
    r *= (M.m[0][0] + M.m[1][1]); // Escala hacia adentro
    r *= (M.m[1][1] + M.m[0][0]); // Escala hacia afuera

	//Rotacion del radio
    r *= sqrtf(M.m[0][0] * M.m[0][0] + M.m[1][0] * M.m[1][0]); // Rotacion del radio
    r *= sqrtf(M.m[0][1] * M.m[0][1] + M.m[1][1] * M.m[1][1]); // Rotacion del radio

	// Transformar centro y radio
	int x, y;
	TransformPoint(M, cx, cy, x, y);
    BRHCircle(x, y, r);
}
