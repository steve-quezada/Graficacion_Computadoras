// Representa un punto o dirección en el espacio 3D.
// Operaciones: Suma, resta, multiplicación, producto punto, producto cruz,
//              magnitud y normalización.

#pragma once
#include <cmath>
#include <cassert>

struct Vector3D
{
    float x{}, y{}, z{};

    Vector3D() = default;
    Vector3D(float x, float y, float z) : x{x}, y{y}, z{z} {}

    // Acceso por índice
    float &operator[](int i)
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    // Versión de solo lectura
    const float &operator[](int i) const
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    // Suma
    Vector3D operator+(const Vector3D &v) const
    {
        return {x + v.x, y + v.y, z + v.z};
    }

    // Resta
    Vector3D operator-(const Vector3D &v) const
    {
        return {x - v.x, y - v.y, z - v.z};
    }

    // Multiplicación por escalar
    Vector3D operator*(float s) const
    {
        return {x * s, y * s, z * s};
    }

    // Negación
    Vector3D operator-() const
    {
        return {-x, -y, -z};
    }

    // Suma acumulada
    Vector3D &operator+=(const Vector3D &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // Resta acumulada
    Vector3D &operator-=(const Vector3D &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Multiplicación acumulada por escalar
    Vector3D &operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    // Producto punto
    float dot(const Vector3D &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    // Producto cruz
    Vector3D cross(const Vector3D &v) const
    {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x};
    }

    // Magnitud
    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalizar
    Vector3D normalized() const
    {
        float len = length();
        return {x / len, y / len, z / len};
    }
};
