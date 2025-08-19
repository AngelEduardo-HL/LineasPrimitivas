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
