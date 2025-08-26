#pragma once
#include "Geometry.h"

class Ball
{
public:
	void BallOnCollision(float cx1, float cy1); //Detecta la colision de la pelota
	void Distancia(float cx1, float cx2, float cy1, float cy2); //Calcula la distancia respecto a otro GameObjet
	void ball(float cx1, float cy1); //Dibuja la pelota

private:
};

