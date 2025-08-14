#pragma once
#include <iostream>
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

    // ---- Dibujo de lineas base ----
    void DDALine(int X1, int Y1, int X2, int Y2, Color col = GREEN);
    void BRHLine(int X1, int Y1, int X2, int Y2, Color col = YELLOW);

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

public:
    virtual ~Geometry() = default;
};
