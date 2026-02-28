#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Author: @steve-quezada

/**
 * @brief Estructura que representa un punto 2D
 */
struct Punto {
    float x, y;
};

/**
 * @brief Clase para dibujar figuras geométricas píxel a píxel usando SFML.
 *
 * Todas las figuras se dibujan mediante rectángulos de 1x1 píxel,
 * con interpolación lineal para líneas y ecuaciones paramétricas para curvas.
 */
class Dibujo {
private:
    std::vector<sf::RectangleShape*> pixeles;

public:
    Dibujo();
    ~Dibujo();

    // ================
    //  Primitiva base
    // ================

    /**
     * @brief Dibuja un píxel (1x1) en la posición indicada.
     */
    void drawPixel(float x, float y, sf::Color color);

    // =================
    //  Figuras básicas
    // =================

    /**
     * @brief Dibuja una línea entre dos puntos usando interpolación lineal.
     * P(t) = (1-t)*P0 + t*P1, con t en [0, 1]
     */
    void drawLine(float x0, float y0, float x1, float y1, sf::Color color);

    /**
     * @brief Dibuja un cuadrado (contorno) en la posición (x,y) con lado 'size'.
     */
    void drawSquare(float x, float y, float size, sf::Color color);

    /**
     * @brief Dibuja un cuadrado relleno en la posición (x,y) con lado 'size'.
     */
    void drawFilledSquare(float x, float y, float size, sf::Color color);

    /**
     * @brief Dibuja un triángulo (contorno) definido por 3 vértices.
     */
    void drawTriangle(float x1, float y1, float x2, float y2,
                      float x3, float y3, sf::Color color);

    /**
     * @brief Dibuja un círculo (contorno) con centro (cx,cy) y radio r.
     * Usa ecuación paramétrica: x = cx + r*cos(t), y = cy + r*sin(t)
     */
    void drawCircle(float cx, float cy, float r, sf::Color color);

    /**
     * @brief Dibuja un círculo relleno usando líneas horizontales (scanline).
     */
    void drawFilledCircle(float cx, float cy, float r, sf::Color color);

    /**
     * @brief Dibuja un polígono regular de n lados inscrito en un círculo.
     * @param n Número de lados (debe ser >= 3)
     */
    void drawPolygon(float cx, float cy, float r, int n, sf::Color color);

    // ==================
    //  Curvas de Bézier
    // ==================

    /**
     * @brief Curva de Bézier cuadrática (3 puntos de control).
     * B(t) = (1-t)²P0 + 2(1-t)tP1 + t²P2
     */
    void drawBezierQuadratic(Punto p0, Punto p1, Punto p2, sf::Color color);

    /**
     * @brief Curva de Bézier cúbica (4 puntos de control).
     * B(t) = (1-t)³P0 + 3(1-t)²tP1 + 3(1-t)t²P2 + t³P3
     */
    void drawBezierCubic(Punto p0, Punto p1, Punto p2, Punto p3, sf::Color color);

    // ======================
    //  Algoritmo de Chaikin
    // ======================

    /**
     * @brief Aplica el algoritmo de Chaikin para suavizar una polilínea.
     * En cada iteración, para cada segmento P_i → P_{i+1}:
     *   Q_i = 3/4 * P_i + 1/4 * P_{i+1}
     *   R_i = 1/4 * P_i + 3/4 * P_{i+1}
     * @param points Puntos de control originales
     * @param iterations Número de iteraciones de subdivisión
     * @return Vector de puntos suavizados
     */
    std::vector<Punto> chaikin(const std::vector<Punto>& points, int iterations);

    /**
     * @brief Dibuja una polilínea suavizada con el algoritmo de Chaikin.
     */
    void drawChaikin(const std::vector<Punto>& points, int iterations, sf::Color color);

    // ============
    //  Utilidades
    // ============

    /**
     * @brief Dibuja una polilínea conectando una secuencia de puntos.
     */
    void drawPolyline(const std::vector<Punto>& points, sf::Color color);

    /**
     * @brief Elimina todos los píxeles almacenados y libera memoria.
     */
    void clear();

    /**
     * @brief Dibuja todos los píxeles almacenados en la ventana.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Dibuja solo los primeros N píxeles (para animación progresiva).
     * @param window Ventana donde dibujar
     * @param maxPixels Cantidad máxima de píxeles a dibujar (0 a pixelCount)
     */
    void drawPartial(sf::RenderWindow& window, size_t maxPixels);

    /**
     * @brief Retorna la cantidad de píxeles almacenados.
     */
    size_t getPixelCount() const;
};
