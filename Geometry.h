#pragma once
#include "raylib.h"
#include <vector>
#include <algorithm>

// Clase base para geometria 2D
class Geometry
{
protected:
    struct Point { int x, y; };
    std::vector<Point> pts; // puntos de borde de la figura actual

    // ---- Guardado de puntos ----
public:
    void ClearPoints();
    void SavePoint(int x, int y);

    struct Mat3 { float m[3][3]; }; // Matriz 3x3 para transformaciones 2D
    static Mat3 TRS(float tx, float ty, float grados, float sx, float sy, float px, float py);

	//---- Matris de traslacion ----
	static Mat3 Traslacion(float tx, float ty); // Matriz de traslacion
	static void Mat3xMat3(const Mat3& a, const Mat3& b, Mat3& out); // Multiplicacion de matrices 3x3
	static void TransformPoint(const Mat3& T, int x, int y, int& outx, int& outy); // Transformacion de punto con matriz 3x3

	//---- Matriz de rotacion ----
    static Mat3 Rotacion(float grados);             
    static Mat3 RotacionPivote(float grados, float cx, float cy);

	//---- Matriz de Escalado ----
	static Mat3 Escala(float sx, float sy); // Matriz de escalado
    static Mat3 EscalaPivote(float sx, float sy, float cx, float cy);

    // ---- Dibujo de lineas base ----
    void DDALine(int X1, int Y1, int X2, int Y2, Color col = BLACK);
    void BRHLine(int X1, int Y1, int X2, int Y2, Color col = BLACK);

    // ---- Rasterizacion generica por eje Y ----
    void FillScanlineY(Color col);

    // ---- Relleno especifico de circulo por filas ----
    void CircleScanlineY(int cx, int cy, int r, Color col);

    // ---- Dibujo de contornos compuestos ----
    void DDASquare(int X1, int Y1, int X2, int Y2);
    void BRHSquare(int X1, int Y1, int X2, int Y2);

    void DDATriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3);
    void BRHTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3);

    void DDACircle(float cx, float cy, float r);
    void BRHCircle(int cx, int cy, float r);

	//---- AABB ----
public:
    struct AABB 
    {
        int xmin, ymin, xmax, ymax;
    };

	// Crear AABB a partir de dos puntos
    static inline AABB MakeAABB(int x1, int y1, int x2, int y2) 
    {
        return { std::min(x1,x2), std::min(y1,y2), std::max(x1,x2), std::max(y1,y2) };
    }
	// Crear AABB a partir de un centro y radio
    static inline bool AABBOverlap(const AABB& a, const AABB& b) 
    {
        return !(b.xmin > a.xmax || a.xmin > b.xmax ||
            b.ymin > a.ymax || a.ymin > b.ymax);
    }
	// Crear AABB a partir de los puntos guardados
    static inline AABB Inflate(const AABB& a, int pad) 
    {
        return { a.xmin - pad, a.ymin - pad, a.xmax + pad, a.ymax + pad };
    }

    virtual ~Geometry() = default;


protected:
    AABB ComputeAABBFromSavedPts() const 
    {
        AABB b{ 0,0,0,0 };
        if (pts.empty()) return b;

        b.xmin = b.xmax = pts[0].x;
        b.ymin = b.ymax = pts[0].y;

        for (size_t i = 1; i < pts.size(); ++i) {
            const auto& p = pts[i];
            if (p.x < b.xmin) b.xmin = p.x;
            if (p.x > b.xmax) b.xmax = p.x;
            if (p.y < b.ymin) b.ymin = p.y;
            if (p.y > b.ymax) b.ymax = p.y;
        }
        return b;
    }
};
