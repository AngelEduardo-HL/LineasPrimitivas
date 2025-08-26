#pragma once
#include "raylib.h"
#include "Geometry.h"

struct CollisionInfo {
    float nx{ 0 }, ny{ 0 };     // normal unitaria (desde superficie hacia la bola)
    float penetration{ 0 };   // cuánto se empujó
    float cx{ 0 }, cy{ 0 };     // punto de contacto (mundo)
};

class Ball; // fwd

class GameObject {
public:
    // Transform
    float tx{ 0 }, ty{ 0 };      // traslación
    float rotDeg{ 0 };         // rotación (grados)
    float sx{ 1 }, sy{ 1 };      // escala
    // Pivote para S/R
    float px{ 0 }, py{ 0 };      // pivote (mundo), por defecto (0,0)

    virtual ~GameObject() = default;

    virtual void Update(float dt) {}
    virtual void Draw() {}

    // Colisión "pull" (por default no colisiona)
    virtual bool CheckCollision(const Ball& b, CollisionInfo& info) { (void)b; (void)info; return false; }
    virtual void OnCollision(Ball& b, const CollisionInfo& info) { (void)b; (void)info; }

protected:
    // Construye M = S_pivote * R_pivote * T (vector-fila)
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
