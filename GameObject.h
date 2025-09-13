#pragma once
#include "Geometry.h"

struct CollisionInfo {
	float nx{ 0 }, ny{ 0 };// normal unitario de colisión
	float penetration{ 0 };// profundidad de penetración
	float cx{ 0 }, cy{ 0 };// punto de contacto
};

class Ball; // fwd

class GameObject {
public:
    // Transformación en mundo
    float tx{ 0 }, ty{ 0 };   // traslación
    float rotDeg{ 0 };      // rotación en grados
    float sx{ 1 }, sy{ 1 };   // escala
    float px{ 0 }, py{ 0 };   // pivote

    virtual ~GameObject() = default;
    virtual void Update(float dt) {}
    virtual void Draw() {}
    virtual bool CheckCollision(const Ball& b, CollisionInfo& info) { (void)b; (void)info; return false; }
    virtual void OnCollision(Ball& b, const CollisionInfo& info) { (void)b; (void)info; }

protected:
	// Matriz de transformación combinada
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
