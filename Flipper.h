#pragma once
#include "GameObject.h"
#include "Cuadrado.h"

class Flipper : public GameObject {
public:
    float length{ 140.f };   // positivo = hacia +X; para el derecho pon negativo
    float width{ 18.f };     // grosor visual
    float restAngle{ -25.f };
    float hitAngle{ 35.f };
    float speed{ 600.f };    // deg/s
    bool  pressed{ false };

    Cuadrado drawer;

    void Update(float dt) override {
        float target = pressed ? hitAngle : restAngle;
        if (rotDeg < target) { rotDeg += speed * dt; if (rotDeg > target) rotDeg = target; }
        else if (rotDeg > target) { rotDeg -= speed * dt; if (rotDeg < target) rotDeg = target; }
    }

    void Draw() override {
        // rectángulo delgado anclado al pivote
        int X1 = (int)(px + 0.5f);
        int Y1 = (int)(py - width * 0.5f + 0.5f);
        int X2 = (int)(px + length + 0.5f);
        int Y2 = (int)(py + width * 0.5f + 0.5f);
        drawer.DrawDDA(BuildM(), X1, Y1, X2, Y2);
    }

    // Obtiene los endpoints del eje central tras rotación
    void Segment(float& ax, float& ay, float& bx, float& by) const {
        Geometry::Mat3 M = const_cast<Flipper*>(this)->BuildM();
        int Axi = (int)(px + 0.5f), Ayi = (int)(py + 0.5f);
        int Bxi = (int)(px + length + 0.5f), Byi = (int)(py + 0.5f);
        int Axr, Ayr, Bxr, Byr;
        Geometry::TransformPoint(M, Axi, Ayi, Axr, Ayr);
        Geometry::TransformPoint(M, Bxi, Byi, Bxr, Byr);
        ax = (float)Axr; ay = (float)Ayr; bx = (float)Bxr; by = (float)Byr;
    }

    // Colisión como “cápsula” muy delgada (segmento + radio = width/2)
    bool CheckCollision(const Ball& b, CollisionInfo& info) override;
    void OnCollision(Ball& b, const CollisionInfo& info) override;
};
