#include "Geometry.h"

// ---------------- Guardado de Puntos ----------------
void Geometry::ClearPoints()
{
    pts.clear();
}

void Geometry::SavePoint(int x, int y)
{
    pts.push_back({ x, y });
}

// ---------------- Matriz de Traslacion ----------------
Geometry::Mat3

Geometry::Traslacion(float tx, float ty)
{
	// Matriz identidad
	Mat3 T = { {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{tx,   ty,   1.0f}
	} };
	return T;
}

// Multiplica dos matrices 3x3 y guarda el resultado en 'out'
void Geometry::Mat3xMat3(const Mat3& a, const Mat3& b, Mat3& out)
{
	// Multiplicacion de matrices 3x3
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
			out.m[i][j] = a.m[i][0] * b.m[0][j] 
                         +a.m[i][1] * b.m[1][j] 
                         +a.m[i][2] * b.m[2][j];
        }
    }	
}

// Transforma un punto (x, y) usando una matriz de transformacion 3x3
void Geometry::TransformPoint(const Mat3& T, int x, int y, int& outx, int& outy)
{
	// Multiplicar el punto (x, y, 1) por la matriz T
    float tx = x * T.m[0][0] + y * T.m[1][0] + 1.0f * T.m[2][0];
    float ty = x * T.m[0][1] + y * T.m[1][1] + 1.0f * T.m[2][1];
    outx = (int)(tx + 0.5f);
    outy = (int)(ty + 0.5f);

}

//------------------- Matriz de Rotacion ----------------
Geometry::Mat3 Geometry::Rotacion(float grados)
{
    // grados -> radianes
    float rad = grados * (PI / 180.0f);
    float c = cosf(rad);
    float s = sinf(rad);

    Mat3 R = { {
        {  c,   s, 0.0f },   // columna x
        { -s,   c, 0.0f },   // columna y
        { 0.0f, 0.0f, 1.0f }
    } };
    return R;
}

Geometry::Mat3 Geometry::RotacionPivote(float grados, float cx, float cy)
{
     Mat3 A = Traslacion(-cx, -cy);
    Mat3 B = Rotacion(grados);
    Mat3 C = Traslacion(cx, cy);

    Mat3 AB, ABC;
    Mat3xMat3(A, B, AB);
    Mat3xMat3(AB, C, ABC);
    return ABC;
}
// ---------------- Matriz de Escalado ----------------
Geometry::Mat3 Geometry::Escala(float sx, float sy)
{
	Mat3 S = { {
		{ sx, 0.0f, 0.0f },
		{ 0.0f, sy, 0.0f },
		{ 0.0f, 0.0f, 1.0f }
	} };
	return S;
}

Geometry::Mat3 Geometry::EscalaPivote(float sx, float sy, float cx, float cy)
{
	Mat3 A = Traslacion(-cx, -cy);
	Mat3 B = Escala(sx, sy);
	Mat3 C = Traslacion(cx, cy);
	Mat3 AB, ABC;

	Mat3xMat3(A, B, AB);
	Mat3xMat3(AB, C, ABC);
	return ABC;
}

// ---------------- Matriz compuesta TRS ----------------
Geometry::Mat3 Geometry::TRS(float tx, float ty, float grados, float sx, float sy, float px, float py)
{
	Mat3 T = Traslacion(tx, ty);
	Mat3 R = RotacionPivote(grados, px, py);
	Mat3 S = EscalaPivote(sx, sy, px, py);
	Mat3 TR, TRS;
	Mat3xMat3(T, R, TR);
	Mat3xMat3(TR, S, TRS);
	return TRS;
}

// ---------------- Lineas base ----------------
void Geometry::DDALine(int X1, int Y1, int X2, int Y2, Color col)
{
    float dx = X2 - X1;
    float dy = Y2 - Y1;
    float absdx = (dx < 0) ? -dx : dx;
    float absdy = (dy < 0) ? -dy : dy;
    int steps = (absdx > absdy) ? (int)absdx : (int)absdy;
    if (steps <= 0) { DrawPixel(X1, Y1, col); return; }

    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = (float)X1;
    float y = (float)Y1;

    for (int i = 0; i <= steps; ++i)
    {
        DrawPixel((int)(x + 0.5f), (int)(y + 0.5f), col);
        x += xInc; y += yInc;
    }
}

void Geometry::BRHLine(int X1, int Y1, int X2, int Y2, Color col)
{
    if (X1 == X2 && Y1 == Y2) { DrawPixel(X1, Y1, col); return; }

    int dx = (X2 > X1) ? (X2 - X1) : (X1 - X2);
    int dy = (Y2 > Y1) ? (Y2 - Y1) : (Y1 - Y2);
    int sx = (X1 < X2) ? 1 : -1;
    int sy = (Y1 < Y2) ? 1 : -1;

    int x = X1, y = Y1;

    if (dx >= dy)
    {
        int p = 2 * dy - dx;
        while (true)
        {
            DrawPixel(x, y, col);
            if (x == X2) break;
            x += sx;
            if (p >= 0) { y += sy; p += 2 * (dy - dx); }
            else { p += 2 * dy; }
        }
    }
    else
    {
        int p = 2 * dx - dy;
        while (true)
        {
            DrawPixel(x, y, col);
            if (y == Y2) break;
            y += sy;
            if (p >= 0) { x += sx; p += 2 * (dx - dy); }
            else { p += 2 * dx; }
        }
    }
}

// ---------------- Rasterizacion por Y ----------------
void Geometry::FillScanlineY(Color col)
{
    if (pts.empty()) return;

    int ymin = pts[0].y, ymax = pts[0].y;
    for (size_t i = 1; i < pts.size(); ++i)
    {
        if (pts[i].y < ymin) ymin = pts[i].y;
        if (pts[i].y > ymax) ymax = pts[i].y;
    }

    for (int y = ymin; y <= ymax; ++y)
    {
        std::vector<int> xs;
        xs.reserve(64);

        for (const auto& p : pts)
            if (p.y == y) xs.push_back(p.x);

        if (xs.empty()) continue;

        std::sort(xs.begin(), xs.end());
        if ((xs.size() % 2) != 0) xs.push_back(xs.back());

        for (size_t k = 0; k + 1 < xs.size(); k += 2)
        {
            int x0 = xs[k], x1 = xs[k + 1];
            if (x1 < x0) std::swap(x0, x1);
            DrawLine(x0, y, x1, y, col);
        }
    }
}

// ---------------- Relleno especifico para circulo ----------------
void Geometry::CircleScanlineY(int cx, int cy, int r, Color col)
{
    if (r <= 0) return;

    for (int dy = 0; dy <= r; ++dy)
    {
        int dx = (int)(sqrtf((float)(r * r - dy * dy)) + 0.0f);

        DrawLine(cx - dx, cy - dy, cx + dx, cy - dy, col);
        if (dy > 0)
            DrawLine(cx - dx, cy + dy, cx + dx, cy + dy, col);
    }
}

// ---------------- Contornos ----------------
void Geometry::DDASquare(int X1, int Y1, int X2, int Y2)
{
    DDALine(X1, Y1, X2, Y1);
    DDALine(X2, Y1, X2, Y2);
    DDALine(X2, Y2, X1, Y2);
    DDALine(X1, Y2, X1, Y1);
}

void Geometry::DDATriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    DDALine(X1, Y1, X2, Y2);
    DDALine(X2, Y2, X3, Y3);
    DDALine(X3, Y3, X1, Y1);
}

void Geometry::BRHSquare(int X1, int Y1, int X2, int Y2)
{
    BRHLine(X1, Y1, X2, Y1);
    BRHLine(X2, Y1, X2, Y2);
    BRHLine(X2, Y2, X1, Y2);
    BRHLine(X1, Y2, X1, Y1);
}

void Geometry::BRHTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
    BRHLine(X1, Y1, X2, Y2);
    BRHLine(X2, Y2, X3, Y3);
    BRHLine(X3, Y3, X1, Y1);
}

// ---------------- Contorno para Circulo ----------------
void Geometry::DDACircle(float cx, float cy, float r)
{
    if (r <= 0.0f) return;
    float dtheta = 1.0f / r;
    float cosd = cosf(dtheta), sind = sinf(dtheta);
    float x = r, y = 0.0f;

    int steps = (int)(2.0f * PI / dtheta) + 1;
    for (int i = 0; i < steps; ++i)
    {
        DrawPixel((int)(cx + x + 0.5f), (int)(cy + y + 0.5f), GREEN);
        float xn = x * cosd - y * sind;
        float yn = x * sind + y * cosd;
        x = xn; y = yn;
    }
}

void Geometry::BRHCircle(int cx, int cy, float r)
{
    int R = (int)(r + 0.5f);
    if (R <= 0) return;

    int x = 0, y = R;
    int d = 1 - R;

    auto draw8 = [&](int xx, int yy)
        {
            DrawPixel(cx + xx, cy + yy, YELLOW);
            DrawPixel(cx - xx, cy + yy, YELLOW);
            DrawPixel(cx + xx, cy - yy, YELLOW);
            DrawPixel(cx - xx, cy - yy, YELLOW);
            DrawPixel(cx + yy, cy + xx, YELLOW);
            DrawPixel(cx - yy, cy + xx, YELLOW);
            DrawPixel(cx + yy, cy - xx, YELLOW);
            DrawPixel(cx - yy, cy - xx, YELLOW);
        };

    while (x <= y)
    {
        draw8(x, y);
        if (d < 0) d += 2 * x + 3;
        else { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}