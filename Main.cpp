#include <iostream>
#include "raylib.h"

#include "Geometry.h"
#include "Cuadrado.h"
#include "Triangulo.h"
#include "Circulo.h"


// ======================= Main =======================
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "GigaChadWindow");
    SetTargetFPS(60);

    // Inicializar geometría
    Cuadrado square;
    Triangulo triangle;
    Circulo circle;

    // Transformaciones base (traslaciones fijas)
    Geometry::Mat3 TDDA = Geometry::Traslacion(10.f, 10.f);        // DDA: desplazamiento
    Geometry::Mat3 TBRH = Geometry::Traslacion(-100.f, 100.f);     // BRH: desplazamiento

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Rotación animada
        float ang = GetTime() * 30.0f;

        // Escala (uniforme para que el círculo no se vuelva elipse)
        float sx = 1.0f + 0.25f * sinf(GetTime() * 2.0f);
        float sy = sx;

        // ---- Coordenadas originales ----
        // DDA

        // Cuadrado
        int sx1 = screenWidth / 2, sy1 = screenHeight / 2;
        int sx2 = 600, sy2 = 600;

        // Triángulo DDA
        int tdx1 = screenWidth / 2, tdy1 = screenHeight / 2;
        int tdx2 = screenWidth / 2 + 200, tdy2 = screenHeight / 2;
        int tdx3 = screenWidth / 2 + 100, tdy3 = screenHeight / 2 - 150;

        // Círculo DDA
        int ccx = 500, ccy = 700; float rr = 30.f;

        // BRH

        // Cuadrado
        int bx1 = screenWidth / 2, by1 = screenHeight / 2;
        int bx2 = 200, by2 = 200;

        // Triángulo BRH
        int btx1 = 400, bty1 = 200;
        int btx2 = 300, bty2 = 50;
        int btx3 = 200, bty3 = 200;

        // Círculo BRH
        int bcx = 300, bcy = 500; float brr = 30.f;

        // ======= Pivotes (centros/centroides) =======
        // DDA
        float pivSx = (sx1 + sx2) * 0.5f, pivQy = (sy1 + sy2) * 0.5f;                       // centro cuadrado DDA
        float pivTx = (tdx1 + tdx2 + tdx3) / 3.0f, pivTy = (tdy1 + tdy2 + tdy3) / 3.0f;     // centroide triángulo DDA
        float pivCx = (float)ccx, pivCy = (float)ccy;                    // centro círculo DDA

        // BRH
        float pivSbx = (bx1 + bx2) * 0.5f, pivQby = (by1 + by2) * 0.5f;                       // centro cuadrado BRH
        float pivTbx = (btx1 + btx2 + btx3) / 3.0f, pivTby = (bty1 + bty2 + bty3) / 3.0f;     // centroide triángulo BRH
        float pivCbx = (float)bcx, pivCby = (float)bcy;                     // centro círculo BRH

        // ======= Rotaciones por pivote =======
        Geometry::Mat3 RDDA_S = Geometry::RotacionPivote(ang, pivSx, pivQy);
        Geometry::Mat3 RDDA_T = Geometry::RotacionPivote(ang, pivTx, pivTy);
        Geometry::Mat3 RDDA_C = Geometry::RotacionPivote(ang, pivCx, pivCy);

        Geometry::Mat3 RBRH_S = Geometry::RotacionPivote(ang, pivSbx, pivQby);
        Geometry::Mat3 RBRH_T = Geometry::RotacionPivote(ang, pivTbx, pivTby);
        Geometry::Mat3 RBRH_C = Geometry::RotacionPivote(ang, pivCbx, pivCby);

        // ======= Escalados por pivote =======
        Geometry::Mat3 SDDA_S = Geometry::EscalaPivote(sx, sy, pivSx, pivQy);
        Geometry::Mat3 SDDA_T = Geometry::EscalaPivote(sx, sy, pivTx, pivTy);
        Geometry::Mat3 SDDA_C = Geometry::EscalaPivote(sx, sy, pivCx, pivCy);

        Geometry::Mat3 SBRH_S = Geometry::EscalaPivote(sx, sy, pivSbx, pivQby);
        Geometry::Mat3 SBRH_T = Geometry::EscalaPivote(sx, sy, pivTbx, pivTby);
        Geometry::Mat3 SBRH_C = Geometry::EscalaPivote(sx, sy, pivCbx, pivCby);

        // ======= Composición (vector-fila): M = S * R * T =======
        Geometry::Mat3 SR;

        Geometry::Mat3 M_DDA_S, M_DDA_T, M_DDA_C;
        Geometry::Mat3xMat3(SDDA_S, RDDA_S, SR); Geometry::Mat3xMat3(SR, TDDA, M_DDA_S);
        Geometry::Mat3xMat3(SDDA_T, RDDA_T, SR); Geometry::Mat3xMat3(SR, TDDA, M_DDA_T);
        Geometry::Mat3xMat3(SDDA_C, RDDA_C, SR); Geometry::Mat3xMat3(SR, TDDA, M_DDA_C);

        Geometry::Mat3 M_BRH_S, M_BRH_T, M_BRH_C;
        Geometry::Mat3xMat3(SBRH_S, RBRH_S, SR); Geometry::Mat3xMat3(SR, TBRH, M_BRH_S);
        Geometry::Mat3xMat3(SBRH_T, RBRH_T, SR); Geometry::Mat3xMat3(SR, TBRH, M_BRH_T);
        Geometry::Mat3xMat3(SBRH_C, RBRH_C, SR); Geometry::Mat3xMat3(SR, TBRH, M_BRH_C);

        // ========================= DIBUJO =========================
        // ---- Casita con DDA ----
        // Cuadrado
        square.DrawDDA(M_DDA_S, sx1, sy1, sx2, sy2);
        // Triángulo
        triangle.DrawDDA(M_DDA_T, tdx1, tdy1, tdx2, tdy2, tdx3, tdy3);
        // Círculo
        circle.DrawDDA(M_DDA_C, ccx, ccy, rr);

        // ---- Casita con BRH ----
        square.DrawBRH(M_BRH_S, bx1, by1, bx2, by2);
        triangle.DrawBRH(M_BRH_T, btx1, bty1, btx2, bty2, btx3, bty3);
        circle.DrawBRH(M_BRH_C, bcx, bcy, brr);

        // ---- Coloreado ----
        square.FillBRH(M_BRH_S, bx1, by1, bx2, by2);
        triangle.FillBRH(M_BRH_T, btx1, bty1, btx2, bty2, btx3, bty3);
        circle.FillBRH(M_BRH_C, bcx, bcy, brr);

        square.FillDDA(M_DDA_S, sx1, sy1, sx2, sy2);
        triangle.FillDDA(M_DDA_T, tdx1, tdy1, tdx2, tdy2, tdx3, tdy3);
        circle.FillDDA(M_DDA_C, ccx, ccy, rr);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
