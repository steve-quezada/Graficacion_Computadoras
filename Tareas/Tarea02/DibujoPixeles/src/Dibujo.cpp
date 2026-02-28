#include "Dibujo.h"
#include <algorithm>

// Author: @steve-quezada

constexpr float PI = 3.14159265358979f;

// =========================
// Constructor / Destructor
// =========================

Dibujo::Dibujo() {}

Dibujo::~Dibujo() {
    clear();
}

// =============================================
// Primitiva base: un píxel (rectángulo de 1x1)
// =============================================

void Dibujo::drawPixel(float x, float y, sf::Color color) {
    sf::RectangleShape* pixel = new sf::RectangleShape({1.f, 1.f});
    pixel->setPosition({x, y});
    pixel->setFillColor(color);
    pixeles.push_back(pixel);
}

// =======================================================
// Línea por interpolación lineal: P(t) = (1-t)*P0 + t*P1
// =======================================================

void Dibujo::drawLine(float x0, float y0, float x1, float y1, sf::Color color) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float dist = std::sqrt(dx * dx + dy * dy);
    int steps = static_cast<int>(dist);
    if (steps < 1) steps = 1;

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        float x = (1.f - t) * x0 + t * x1;
        float y = (1.f - t) * y0 + t * y1;
        drawPixel(x, y, color);
    }
}

// ==============================
// Cuadrado (contorno): 4 líneas
// ==============================

void Dibujo::drawSquare(float x, float y, float size, sf::Color color) {
    drawLine(x, y, x + size, y, color);                 // superior
    drawLine(x + size, y, x + size, y + size, color);   // derecha
    drawLine(x + size, y + size, x, y + size, color);   // inferior
    drawLine(x, y + size, x, y, color);                 // izquierda
}

// ======================================
// Cuadrado relleno: líneas horizontales
// ======================================

void Dibujo::drawFilledSquare(float x, float y, float size, sf::Color color) {
    for (float j = 0; j <= size; j += 1.f) {
        drawLine(x, y + j, x + size, y + j, color);
    }
}

// ===============================
// Triángulo (contorno): 3 líneas
// ===============================

void Dibujo::drawTriangle(float x1, float y1, float x2, float y2,
                           float x3, float y3, sf::Color color) {
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);
}

// =========================================
// Círculo (contorno): ecuación paramétrica
// x = cx + r*cos(t), y = cy + r*sin(t)
// =========================================

void Dibujo::drawCircle(float cx, float cy, float r, sf::Color color) {
    int steps = std::max(60, static_cast<int>(2.f * PI * r));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(steps) * 2.f * PI;
        float x = cx + r * std::cos(t);
        float y = cy + r * std::sin(t);
        drawPixel(x, y, color);
    }
}

// =====================================
// Círculo relleno: scanline horizontal
// =====================================

void Dibujo::drawFilledCircle(float cx, float cy, float r, sf::Color color) {
    for (float dy = -r; dy <= r; dy += 1.f) {
        float dx = std::sqrt(r * r - dy * dy);
        drawLine(cx - dx, cy + dy, cx + dx, cy + dy, color);
    }
}

// ============================
// Polígono regular de n lados
// ============================

void Dibujo::drawPolygon(float cx, float cy, float r, int n, sf::Color color) {
    if (n < 3) return;

    for (int i = 0; i < n; ++i) {
        float angle1 = 2.f * PI * i / n - PI / 2.f;
        float angle2 = 2.f * PI * (i + 1) / n - PI / 2.f;

        float x1 = cx + r * std::cos(angle1);
        float y1 = cy + r * std::sin(angle1);
        float x2 = cx + r * std::cos(angle2);
        float y2 = cy + r * std::sin(angle2);

        drawLine(x1, y1, x2, y2, color);
    }
}

// ===============================================================
// Curva de Bézier cuadrática: B(t) = (1-t)²P0 + 2(1-t)tP1 + t²P2
// ===============================================================

void Dibujo::drawBezierQuadratic(Punto p0, Punto p1, Punto p2, sf::Color color) {
    float dist = std::sqrt(std::pow(p2.x - p0.x, 2) + std::pow(p2.y - p0.y, 2));
    int steps = std::max(100, static_cast<int>(dist));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        float omt = 1.f - t;

        float x = omt * omt * p0.x + 2.f * omt * t * p1.x + t * t * p2.x;
        float y = omt * omt * p0.y + 2.f * omt * t * p1.y + t * t * p2.y;

        drawPixel(x, y, color);
    }
}

// =========================================================================
// Curva de Bézier cúbica: B(t) = (1-t)³P0 + 3(1-t)²tP1 + 3(1-t)t²P2 + t³P3
// =========================================================================

void Dibujo::drawBezierCubic(Punto p0, Punto p1, Punto p2, Punto p3, sf::Color color) {
    float dist = std::sqrt(std::pow(p3.x - p0.x, 2) + std::pow(p3.y - p0.y, 2));
    int steps = std::max(100, static_cast<int>(dist * 1.5f));

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(steps);
        float omt = 1.f - t;

        float x = omt*omt*omt * p0.x + 3.f*omt*omt*t * p1.x
                + 3.f*omt*t*t * p2.x + t*t*t * p3.x;
        float y = omt*omt*omt * p0.y + 3.f*omt*omt*t * p1.y
                + 3.f*omt*t*t * p2.y + t*t*t * p3.y;

        drawPixel(x, y, color);
    }
}

// ==================================
// Algoritmo de Chaikin
// Para cada segmento P_i → P_{i+1}:
//   Q = 3/4 * P_i + 1/4 * P_{i+1}
//   R = 1/4 * P_i + 3/4 * P_{i+1}
// ==================================

std::vector<Punto> Dibujo::chaikin(const std::vector<Punto>& points, int iterations) {
    if (iterations <= 0 || points.size() < 2) return points;

    std::vector<Punto> result;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        Punto q = {
            0.75f * points[i].x + 0.25f * points[i + 1].x,
            0.75f * points[i].y + 0.25f * points[i + 1].y
        };
        Punto r = {
            0.25f * points[i].x + 0.75f * points[i + 1].x,
            0.25f * points[i].y + 0.75f * points[i + 1].y
        };
        result.push_back(q);
        result.push_back(r);
    }

    return chaikin(result, iterations - 1);
}

void Dibujo::drawChaikin(const std::vector<Punto>& points, int iterations, sf::Color color) {
    std::vector<Punto> smoothed = chaikin(points, iterations);
    drawPolyline(smoothed, color);
}

// ==================================================
// Polilínea: conecta puntos consecutivos con líneas
// ==================================================

void Dibujo::drawPolyline(const std::vector<Punto>& points, sf::Color color) {
    for (size_t i = 0; i < points.size() - 1; ++i) {
        drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
    }
}

// ==========================================
// Limpia todos los píxeles y libera memoria
// ==========================================

void Dibujo::clear() {
    for (auto* pixel : pixeles) {
        delete pixel;
    }
    pixeles.clear();
}

// =======================================
// Dibuja todos los píxeles en la ventana
// =======================================

void Dibujo::draw(sf::RenderWindow& window) {
    for (const auto* pixel : pixeles) {
        window.draw(*pixel);
    }
}

// ===================================
// Dibuja solo los primeros N píxeles 
// ===================================

void Dibujo::drawPartial(sf::RenderWindow& window, size_t maxPixels) {
    size_t limit = std::min(maxPixels, pixeles.size());
    for (size_t i = 0; i < limit; ++i) {
        window.draw(*pixeles[i]);
    }
}

// ============================
// Retorna cantidad de píxeles
// ============================

size_t Dibujo::getPixelCount() const {
    return pixeles.size();
}
