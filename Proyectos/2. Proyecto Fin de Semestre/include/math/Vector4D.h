// Extiende Vector3D con una cuarta componente w (coordenada homogénea).
// w = 1.0 representa un punto, w = 0.0 una dirección.

#pragma once
#include "math/Vector3D.h"

struct Vector4D
{
    float x{}, y{}, z{}, w{};

    Vector4D() = default;
    Vector4D(float x, float y, float z, float w = 1.0f) : x{x}, y{y}, z{z}, w{w} {}

    // Conversión desde Vector3D
    Vector4D(const Vector3D &v, float w = 1.0f) : x{v.x}, y{v.y}, z{v.z}, w{w} {}

    // Conversión a Vector3D usando la coordenada homogénea
    Vector3D toVector3D() const
    {
        if (w != 0.0f && w != 1.0f)
        {
            return {x / w, y / w, z / w};
        }
        return {x, y, z};
    }

    // Acceso por índice
    float &operator[](int i)
    {
        assert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    // Versión de solo lectura
    const float &operator[](int i) const
    {
        assert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    // Suma
    Vector4D operator+(const Vector4D &v) const
    {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    // Resta
    Vector4D operator-(const Vector4D &v) const
    {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    // Multiplicación por escalar
    Vector4D operator*(float s) const
    {
        return {x * s, y * s, z * s, w * s};
    }
};
