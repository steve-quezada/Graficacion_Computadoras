#include <SFML/Graphics.hpp>
#include "Dibujo.h"
#include <vector>
#include <iostream>
#include <cmath>

#ifdef _WIN32
    #include <windows.h>
#endif

// Author: @steve-quezada

// =======================
//  Dibujar gato completo 
// =======================

void dibujarContorno(Dibujo& d)
{
    sf::Color negro   (  0,   0,   0);
    sf::Color blanco  (255, 255, 255);
    sf::Color rosa    (220, 100, 130);   // #DC6482
    sf::Color grisOsc ( 60,  20,  20);  // #3C1414

    // Escala para convertir coordenadas del diseño (Desmos)
    // a píxeles en la ventana SFML:
    //  - SX, SY: pixeles por unidad en X / Y
    //  - CX, CY: centro en píxeles (250,250 a ventana 500×50)
    constexpr float SX = 3.8f;
    constexpr float SY = 1.9f;
    constexpr float CX = 250.f;
    constexpr float CY = 250.f;

    // Convierte punto Desmos → pantalla SFML
    auto P = [&](float x, float y) -> Punto {
        return { CX + x * SX, CY - y * SY };
    };

    /*
     * Relleno por scanlines (rasterización):
     * - Calcula el rango Y del polígono (yMin..yMax) usando floor/ceil
     * - Para cada scanline entera iy, muestrea en el centro del píxel: fy = iy + 0.5
     * - Interseca la scanline con cada arista; para cada arista que cruza fy calcula 
     *   el punto de intersección X
     * - Ordena todas las X de intersección y rellena por pares (xs[k]..xs[k+1])
     * - Usa floor(xs[k]) y ceil(xs[k+1]) al dibujar la línea horizontal.
     */
    auto fillPoly = [&](const std::vector<Punto>& poly, sf::Color col) {
        // número de vértices
        int n = (int)poly.size();
        // inicializar límites Y
        int yMin = INT_MAX, yMax = INT_MIN;
        // calcular yMin,yMax en coordenadas de pantalla
        for (auto& p : poly) {
            yMin = std::min(yMin, (int)std::floor(p.y)); // piso del y del vértice
            yMax = std::max(yMax, (int)std::ceil(p.y));  // techo del y del vértice
        }
        // para cada scanline entera entre yMin y yMax
        for (int iy = yMin; iy <= yMax; iy++) {
            float fy = (float)iy + 0.5f;  // muestreo en el centro del píxel
            std::vector<float> xs;        // lista de intersecciones X con la scanline
            // recorrer aristas del polígono
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;      // índice del siguiente vértice
                float y0 = poly[i].y, y1 = poly[j].y; // Y de cada extremo de la arista
                if (std::abs(y1 - y0) < 0.0001f) continue; // ignorar aristas horizontales
                float lo = std::min(y0, y1), hi = std::max(y0, y1);
                // comprobar si la scanline (fy) cruza el intervalo vertical de la arista
                if (fy > lo && fy <= hi) {
                    float t = (fy - y0) / (y1 - y0); // parámetro de intersección [0,1]
                    // interpolar X en la arista donde cruza la scanline
                    xs.push_back(poly[i].x + t * (poly[j].x - poly[i].x));
                }
            }
            // ordenar intersecciones de izquierda a derecha
            std::sort(xs.begin(), xs.end());
            // rellenar por pares: [xs[0],xs[1]], [xs[2],xs[3]], ...
            for (size_t k = 0; k + 1 < xs.size(); k += 2)
                // usar floor/ceil para asegurar cobertura completa de píxeles
                d.drawLine(std::floor(xs[k]), (float)iy,
                           std::ceil(xs[k+1]),  (float)iy, col);
        }
    };

    /*
     * Relleno de elipse por scanlines:
     * - Recorre las scanlines enteras entre floor(cy-r y) y ceil(cy+ry).
     * - Para cada iy evalúa la distancia vertical al centro usando el punto
     *   muestreado en el centro del píxel: dy = iy + 0.5 - cy.
     * - Con la ecuación de la elipse calcula el semiancho horizontal xw
     *   en esa altura: xw = rx * sqrt(1 - (dy/ry)^2).
     * - Dibuja una línea horizontal desde floor(cx-xw) hasta ceil(cx+xw).
     */
    auto fillEllipse = [&](float cx, float cy, float rx, float ry, sf::Color col) {
        // límite Y entero de la elipse
        int yMin = (int)std::floor(cy - ry);
        int yMax = (int)std::ceil(cy + ry);
        // recorrer cada scanline entera en el rango
        for (int iy = yMin; iy <= yMax; iy++) {
            float dy = (float)iy + 0.5f - cy;    // distancia vertical desde el centro (centro del píxel)
            float rat = 1.f - (dy/ry) * (dy/ry); // valor dentro de la raíz (1 - (y/ry)^2)
            if (rat < 0.f) continue;             // fuera de la elipse en esta scanline
            float xw = rx * std::sqrt(rat);      // semiancho horizontal en esta altura
            // dibujar línea horizontal entre los extremos
            d.drawLine(std::floor(cx - xw), (float)iy,
                       std::ceil (cx + xw), (float)iy, col);
        }
    };

    // =======================
    //  A. CONTORNO PRINCIPAL
    // =======================
    {
        std::vector<Punto> contorno = {
            P(-41,-121), P(-38,-91), P(-34,-60), P(-30,-39), P(-29,-30),
            P(-38,-11),  P(-43, 6),  P(-44, 24), P(-41, 48), P(-36, 67),
            P(-31, 91),  P(-27,115), P(-26,118),
            P(-19,102),  P(-12, 94), P( -6, 97), P(  0, 99), P(  6, 97), P( 13, 92),
            P( 21,102),  P( 27,118),
            P( 32, 91),  P( 36, 67), P( 41, 48), P( 46, 24), P( 46,  6),
            P( 42,-11),  P( 31,-33), P( 34,-54), P( 38,-91), P( 42,-121),
            P(-41,-121)
        };

        // 3 pasadas offset en X e Y → trazo ~2px sin dobles líneas
        for (int ox = 0; ox <= 1; ox++)
            for (int oy = 0; oy <= 1; oy++) {
                std::vector<Punto> off;
                off.reserve(contorno.size());
                for (auto& p : contorno)
                    off.push_back({ std::round(p.x) + ox, std::round(p.y) + oy });
                d.drawPolyline(off, negro);
            }
    }

    // ==============================================
    // B. FONDO OSCURO DE LA BOCA (polígono relleno)
    // ==============================================
    {
        std::vector<Punto> boca = {
            P(-11,17), P(-8,2),  P(-4,-5), P(0,-7),   P(4,-5),  P(8,2),   P(11,17),
            P(10,13),  P(8,10),  P(6,9),   P(5,9),    P(3,11),  P(2,13),  P(1,14),
            P(0,15),   P(-1,14), P(-2,13), P(-3,11),  P(-5,9),  P(-6,9),
            P(-8,10),  P(-10,13)
        };
        fillPoly(boca, grisOsc);
    }

    // ======================================================
    // C. LENGUA — un solo polígono cerrado: relleno + borde
    // ======================================================
    {
        std::vector<Punto> lengua = {
            P(-6, 2), P(6, 2), P(6,-12),
            P(5,-15), P(3,-17), P(0,-18),
            P(-3,-17), P(-5,-15), P(-6,-12)
        };
        // 1° relleno (sin bordes)
        fillPoly(lengua, rosa);
        // 2° borde negro encima (grosor 2px)
        std::vector<Punto> cierre = lengua;
        cierre.push_back(lengua[0]);  // cerrar explícitamente
        for (int g = 0; g < 2; g++) {
            std::vector<Punto> off;
            for (auto& p : cierre) off.push_back({p.x + g*0.5f, p.y + g*0.5f});
            d.drawPolyline(off, negro);
        }
    }

    // ================================================================
    // D. LABIO SUPERIOR  (grosor 3px)
    // ================================================================
    {
        std::vector<Punto> labio = {
            P(-11,17), P(-10,13), P(-8,10),  P(-6,9),  P(-5,9),
            P(-3,11),  P(-2,13),  P(-1,14),  P(0,15),  P(1,14),
            P(2,13),   P(3,11),   P(5,9),    P(6,9),   P(8,10),
            P(10,13),  P(11,17)
        };
        for (int g = 0; g < 3; g++) {
            std::vector<Punto> off;
            for (auto& p : labio) off.push_back({p.x, p.y + (float)g});
            d.drawPolyline(off, negro);
        }
    }

    // =====================================
    // E. OJOS  — elipses rellenas y brillo
    // =====================================
    {
        float axPx = std::sqrt(32.f)  * SX;   // semi-eje X en px
        float ayPx = std::sqrt(105.f) * SY;   // semi-eje Y en px
        Punto oL = P(-18, 35);
        Punto oR = P( 18, 35);

        // 1° relleno negro de cada ojo
        fillEllipse(oL.x, oL.y, axPx, ayPx, negro);
        fillEllipse(oR.x, oR.y, axPx, ayPx, negro);

        // 2° brillo blanco 
        float bRad = 4.5f;
        float bDX  = axPx * 0.38f;  // desplazamiento horizontal
        float bDY  = ayPx * 0.38f;  // desplazamiento vertical
        fillEllipse(oL.x - bDX, oL.y - bDY, bRad, bRad, blanco);
        fillEllipse(oR.x - bDX, oR.y - bDY, bRad, bRad, blanco);
    }

    // ========================
    // G. CEJAS  — 2 segmentos 
    // ========================
    {
        struct Seg { float x0,y0,x1,y1; };
        std::vector<Seg> segs = {
            { P(-29,57).x, P(-29,57).y, P(-19,49).x, P(-19,49).y },
            { P(-19,49).x, P(-19,49).y, P(-10,40).x, P(-10,40).y },
            { P( 29,57).x, P( 29,57).y, P( 19,49).x, P( 19,49).y },
            { P( 19,49).x, P( 19,49).y, P( 10,40).x, P( 10,40).y }
        };
        for (int g = 0; g < 4; g++)
            for (auto& s : segs)
                d.drawLine(s.x0, s.y0 + g*0.7f, s.x1, s.y1 + g*0.7f, negro);
    }

    // ========================
    // H. LÍNEAS DE MOVIMIENTO 
    // ========================
    {
        struct Seg { float x0,y0,x1,y1; };
        std::vector<Seg> segs = {
            { P(-19,-14).x, P(-19,-14).y, P(-11,-14).x, P(-11,-14).y },
            { P(-22,-22).x, P(-22,-22).y, P(-13,-22).x, P(-13,-22).y },
            { P(-18,-30).x, P(-18,-30).y, P(-10,-30).x, P(-10,-30).y }
        };
        for (int g = 0; g < 3; g++)
            for (auto& s : segs)
                d.drawLine(s.x0, s.y0 + g*0.5f, s.x1, s.y1 + g*0.5f, negro);
    }
}


int main()
{
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    sf::RenderWindow window(sf::VideoMode({500, 500}), "Michi");
    window.setFramerateLimit(60);

    // Precalcular todo el gato
    Dibujo dibujoCompleto;
    dibujarContorno(dibujoCompleto);
    
    size_t totalPixeles = dibujoCompleto.getPixelCount();
    size_t pixelesActuales = 0;
    
    bool reproduciendo = false;  // Pausado por defecto
    int velocidad = 50;          // píxeles por frame
    
    std::cout << "\n╔════════════════════╗\n"
              << "║  Control Animación  ║\n"
              << "╚═════════════════════╝\n\n"
              << "Total de píxeles: " << totalPixeles << "\n\n"
              << "Controles:\n"
              << "  Flecha Derecha   → Acelerar    (+20 px/frame)\n"
              << "  Flecha Izquierda → Desacelerar (-20 px/frame)\n"
              << "  ENTER            → Pausar/Reanudar\n"
              << "  R                → Reiniciar\n"
              << "  Cerrar           → Salir\n\n"
              << "Programa Pausado. Presiona ENTER para comenzar...\n\n";

    int frameCount = 0;
    int lastProgress = -1;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                // Flecha derecha: acelerar
                if (keyEvent->code == sf::Keyboard::Key::Right)
                {
                    velocidad = std::min(500, velocidad + 20);
                    std::cout << "➜ Velocidad: " << velocidad << " píxeles/frame (↑)\n";
                }
                // Flecha izquierda: desacelerar
                else if (keyEvent->code == sf::Keyboard::Key::Left)
                {
                    velocidad = std::max(5, velocidad - 20);
                    std::cout << "← Velocidad: " << velocidad << " píxeles/frame (↓)\n";
                }
                // ENTER: pausar/reanudar
                else if (keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    reproduciendo = !reproduciendo;
                    std::cout << (reproduciendo ? "▶ Reproduciendo..." : "⏸ Pausado") << std::endl;
                }
                // R: reiniciar
                else if (keyEvent->code == sf::Keyboard::Key::R)
                {
                    pixelesActuales = 0;
                    reproduciendo = false;
                    lastProgress = -1;
                    frameCount = 0;
                    std::cout << "↻ Reiniciado. Presiona ENTER para comenzar...\n";
                }
            }
        }

        // Incrementar píxeles si está reproduciendo
        if (reproduciendo && pixelesActuales < totalPixeles) {
            pixelesActuales = std::min(pixelesActuales + velocidad, totalPixeles);

            // Mostrar progreso cada 30 frames (~0.5 segundos)
            frameCount++;
            if (frameCount % 30 == 0) {
                int porcentaje = (pixelesActuales * 100) / totalPixeles;
                if (porcentaje != lastProgress) {
                    lastProgress = porcentaje;
                    std::cout << "Progreso: " << porcentaje << "% (" << pixelesActuales 
                              << "/" << totalPixeles << " píxeles)\n";
                }
            }
        }

        // Verificación incondicional de finalización: si alcanzamos o pasamos el total, mostrar 100%
        if (pixelesActuales >= totalPixeles && lastProgress != 100) {
            pixelesActuales = totalPixeles;
            lastProgress = 100;
            reproduciendo = false;
            std::cout << "✔ Animación completada: 100% (" << totalPixeles << "/" << totalPixeles << " píxeles)\n";
        }

        // Renderizar
        window.clear(sf::Color::White);
        dibujoCompleto.drawPartial(window, pixelesActuales);
        window.display();
    }

    return 0;
}
