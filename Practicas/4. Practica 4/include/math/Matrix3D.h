// Matriz 3×3 almacenada en column-major order.
// Operaciones: suma, resta, multiplicación (escalar, matriz, vector),
//              transpuesta, inversa.

#pragma once
#include <cmath>
#include <cassert>
#include "math/Vector3D.h"

struct Matrix3D
{
    float m[3][3]{};

    Matrix3D() = default;

    // Constructor (entrada fila-columna, almacenamiento column-major)
    Matrix3D(float m00, float m01, float m02,
             float m10, float m11, float m12,
             float m20, float m21, float m22)
        : m{{m00, m10, m20},
            {m01, m11, m21},
            {m02, m12, m22}} {}

    // Constructor con 3 vectores columna
    Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c)
        : m{{a[0], a[1], a[2]},
            {b[0], b[1], b[2]},
            {c[0], c[1], c[2]}} {}

    // Acceso por fila, columna
    float &operator()(int row, int col)
    {
        assert(row >= 0 && row <= 2);
        assert(col >= 0 && col <= 2);
        return m[col][row];
    }

    // Versión de solo lectura
    const float &operator()(int row, int col) const
    {
        assert(row >= 0 && row <= 2);
        assert(col >= 0 && col <= 2);
        return m[col][row];
    }

    // Extraer columna como Vector3D
    Vector3D column(int col) const
    {
        assert(col >= 0 && col <= 2);
        return {m[col][0], m[col][1], m[col][2]};
    }

    // Matriz identidad
    static Matrix3D identity()
    {
        return Matrix3D(1, 0, 0,
                        0, 1, 0,
                        0, 0, 1);
    }

    // Suma
    Matrix3D operator+(const Matrix3D &b) const
    {
        Matrix3D result;
        for (int c = 0; c < 3; c++)
            for (int r = 0; r < 3; r++)
                result.m[c][r] = m[c][r] + b.m[c][r];
        return result;
    }

    // Resta
    Matrix3D operator-(const Matrix3D &b) const
    {
        Matrix3D result;
        for (int c = 0; c < 3; c++)
            for (int r = 0; r < 3; r++)
                result.m[c][r] = m[c][r] - b.m[c][r];
        return result;
    }

    // Multiplicación por escalar
    Matrix3D operator*(float s) const
    {
        Matrix3D result;
        for (int c = 0; c < 3; c++)
            for (int r = 0; r < 3; r++)
                result.m[c][r] = m[c][r] * s;
        return result;
    }

    // Multiplicación Matriz * Matriz
    Matrix3D operator*(const Matrix3D &b) const
    {
        Matrix3D result;
        for (int c = 0; c < 3; c++)
            for (int r = 0; r < 3; r++)
            {
                float sum = 0;
                for (int k = 0; k < 3; k++)
                    sum += (*this)(r, k) * b(k, c);
                result(r, c) = sum;
            }
        return result;
    }

    // Multiplicación Matriz * Vector3D
    Vector3D operator*(const Vector3D &v) const
    {
        return {
            (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z,
            (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z,
            (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z};
    }

    // Transpuesta: intercambia filas y columnas
    Matrix3D transpose() const
    {
        Matrix3D result;
        for (int c = 0; c < 3; c++)
            for (int r = 0; r < 3; r++)
                result.m[c][r] = m[r][c];
        return result;
    }

    // Inversa por método de cofactores y adjunta.
    Matrix3D inverse() const
    {
        const Matrix3D &M = *this;

        // Cofactores de cada elemento
        float c00 = M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1);
        float c01 = -(M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0));
        float c02 = M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0);

        float c10 = -(M(0, 1) * M(2, 2) - M(0, 2) * M(2, 1));
        float c11 = M(0, 0) * M(2, 2) - M(0, 2) * M(2, 0);
        float c12 = -(M(0, 0) * M(2, 1) - M(0, 1) * M(2, 0));

        float c20 = M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1);
        float c21 = -(M(0, 0) * M(1, 2) - M(0, 2) * M(1, 0));
        float c22 = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);

        // Determinante en la primera fila
        float det = M(0, 0) * c00 + M(0, 1) * c01 + M(0, 2) * c02;
        assert(std::abs(det) > 1e-6f);
        float invDet = 1.0f / det;

        // Las filas de la inversa son las columnas de la adjunta
        return Matrix3D(
            c00 * invDet, c10 * invDet, c20 * invDet,
            c01 * invDet, c11 * invDet, c21 * invDet,
            c02 * invDet, c12 * invDet, c22 * invDet);
    }
};
