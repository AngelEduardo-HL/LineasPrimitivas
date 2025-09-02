#pragma once
#include "raylib.h"
#include "Geometry.h"

struct CollisionInfo {
    float nx{ 0 }, ny{ 0 };
    float penetration{ 0 };
    float cx{ 0 }, cy{ 0 };
};

class Ball; // fwd

class GameObject {
public:
    // Transformaci�n en mundo
    float tx{ 0 }, ty{ 0 };   // traslaci�n
    float rotDeg{ 0 };      // rotaci�n en grados
    float sx{ 1 }, sy{ 1 };   // escala
    float px{ 0 }, py{ 0 };   // pivote

    virtual ~GameObject() = default;
    virtual void Update(float dt) {}
    virtual void Draw() {}
    virtual bool CheckCollision(const Ball& b, CollisionInfo& info) { (void)b; (void)info; return false; }
    virtual void OnCollision(Ball& b, const CollisionInfo& info) { (void)b; (void)info; }

protected:
    // M = S(piv) * R(piv) * T
    Geometry::Mat3 BuildM() const {
        auto S = Geometry::EscalaPivote(sx, sy, px, py);
        auto R = Geometry::RotacionPivote(rotDeg, px, py);
        auto T = Geometry::Traslacion(tx, ty);
        Geometry::Mat3 SR, SRT;
        Geometry::Mat3xMat3(S, R, SR);
        Geometry::Mat3xMat3(SR, T, SRT);
        return SRT;
    }
};
