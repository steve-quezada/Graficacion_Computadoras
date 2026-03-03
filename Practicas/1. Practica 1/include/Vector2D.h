#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>

namespace math::linear {

    struct Vector2D {
        float x, y;

        Vector2D() : x{0.0f}, y{0.0f} {}

        Vector2D(float a, float b) : x{a}, y{b} {}

        // Nota: He corregido la lógica del if (0 <= i && i <= 1)
        float& operator [](int i) {
            return (i == 0) ? x : y;
        }

        const float& operator [](int i) const {
            return (i == 0) ? x : y;
        }

        Vector2D& operator *= (float s) {
            x *= s; y *= s;
            return *this;
        }

        Vector2D& operator /= (float s) {
            x /= s; y /= s;
            return *this;
        }

        Vector2D& operator += (const Vector2D& v) {
            x += v.x; y += v.y;
            return *this;
        }

        Vector2D& operator -= (const Vector2D& v) {
            x -= v.x; y -= v.y;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }
    };

    // --- FUNCIONES INLINE (Para evitar errores de definición múltiple) ---

    inline Vector2D operator * (const Vector2D& v, float s) {
        return Vector2D{v.x * s, v.y * s};
    }

    inline Vector2D operator / (const Vector2D& v, float s) {
        return Vector2D{v.x / s, v.y / s};
    }

    inline Vector2D operator - (const Vector2D& v) {
        return Vector2D{-v.x, -v.y};
    }

    inline Vector2D operator + (const Vector2D& u, const Vector2D& v) {
        return Vector2D{u.x + v.x, u.y + v.y};
    }

    inline Vector2D operator - (const Vector2D& u, const Vector2D& v) {
        return Vector2D{u.x - v.x, u.y - v.y};
    }

    inline float Magnitude(const Vector2D& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    inline Vector2D Normalize(const Vector2D& v) {
        float magnitude{Magnitude(v)};
        if (magnitude <= 0.00001f) return Vector2D{0.0f, 0.0f}; // Seguridad para flotantes
        return v / magnitude;
    }

} // namespace math::linear

namespace math::functions{

    inline float Lerp(float a, float b, float t){
        //this methods guaranties that x = b when t =1
        return (1-t)*a + (t*b);
    }
};


#endif