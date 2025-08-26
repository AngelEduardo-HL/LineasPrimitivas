#pragma once
#include "Geometry.h"
#include "Cuadrado.h"
#include "Circulo.h"
#include "Triangulo.h"

class GameObjet
{
public:
	void CircleCollision(float cx1, float cx2, float cy1, float cy2); //Detecta las colisiones del Circulo
	void SquareCollision(float sx1, float sx2, float sy1, float sy2); //Detecta las colisiones del Cuadrado
	void TriangleCollision(float tx1, float tx2, float ty1, float ty2); //Detecta las colisones del Triangulo

	void DrawFigure(Cuadrado, Triangulo, Circulo, Color); //Dibuja la Figura que se le pidio

	void PivoteCollision();

private:
};

