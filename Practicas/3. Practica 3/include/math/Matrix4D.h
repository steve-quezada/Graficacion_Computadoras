// Matriz 4×4 almacenada en column-major order.
// Operaciones: multiplicación (matriz×matriz, matriz×vector4D), transpuesta.
// Transformaciones: translate, scale, rotateX/Y/Z.

#pragma once
#include <cmath>
#include <cassert>
#include "math/Vector3D.h"
#include "math/Vector4D.h"

struct Matrix4D
{
    float m[4][4]{};

    // Constructor de Matriz identidad por defecto
    Matrix4D()
    {
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                m[c][r] = (c == r) ? 1.0f : 0.0f;
    }

    // Acceso por fila, columna
    float &operator()(int row, int col)
    {
        assert(row >= 0 && row <= 3);
        assert(col >= 0 && col <= 3);
        return m[col][row];
    }

    // Versión de solo lectura
    float operator()(int row, int col) const
    {
        assert(row >= 0 && row <= 3);
        assert(col >= 0 && col <= 3);
        return m[col][row];
    }

    // Puntero al primer elemento
    const float *value_ptr() const
    {
        return &m[0][0];
    }

    // Multiplicación Matriz * Matriz
    Matrix4D operator*(const Matrix4D &b) const
    {
        Matrix4D result;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
            {
                result.m[c][r] = 0;
                for (int k = 0; k < 4; k++)
                    result.m[c][r] += m[k][r] * b.m[c][k];
            }
        return result;
    }

    // Multiplicación Matriz * Vector4D
    Vector4D operator*(const Vector4D &v) const
    {
        return {
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
            m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w};
    }

    // Transpuesta: intercambia filas y columnas
    Matrix4D transpose() const
    {
        Matrix4D result;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                result.m[c][r] = m[r][c];
        return result;
    }

    // Transformaciones

    // Traslación
    static Matrix4D translate(const Vector3D &t)
    {
        Matrix4D mat;
        mat(0, 3) = t.x;
        mat(1, 3) = t.y;
        mat(2, 3) = t.z;
        return mat;
    }

    // Escalado
    static Matrix4D scale(const Vector3D &s)
    {
        Matrix4D mat;
        mat(0, 0) = s.x;
        mat(1, 1) = s.y;
        mat(2, 2) = s.z;
        return mat;
    }

    // Rotación en eje X (radianes)
    static Matrix4D rotateX(float angle)
    {
        Matrix4D mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat(1, 1) = c;
        mat(1, 2) = -s;
        mat(2, 1) = s;
        mat(2, 2) = c;
        return mat;
    }

    // Rotación en eje Y (radianes)
    static Matrix4D rotateY(float angle)
    {
        Matrix4D mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat(0, 0) = c;
        mat(0, 2) = s;
        mat(2, 0) = -s;
        mat(2, 2) = c;
        return mat;
    }

    // Rotación en eje Z (radianes)
    static Matrix4D rotateZ(float angle)
    {
        Matrix4D mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat(0, 0) = c;
        mat(0, 1) = -s;
        mat(1, 0) = s;
        mat(1, 1) = c;
        return mat;
    }
};
