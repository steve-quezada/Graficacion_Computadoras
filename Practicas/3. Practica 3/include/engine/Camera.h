// Cámara orbital en coordenadas esféricas.
// Encapsula posición, orientación, proyección y la lógica de órbita.

#pragma once
#include <cmath>
#include "math/Vector3D.h"
#include "math/Matrix4D.h"

class Camera
{
public:
    // Crea la cámara con posición esférica y parámetros de proyección
    Camera(const Vector3D &target, float radius, float pitch, float yaw,
           float fov, float near, float far);

    // Orbitar la cámara
    void orbit(float deltaYaw, float deltaPitch);

    // Matrices de vista y proyección
    Matrix4D getViewMatrix() const;
    Matrix4D getProjectionMatrix(float aspect) const;

    // LookAt: construye la matriz de vista que orienta la cámara.
    // Calcula tres ejes ortogonales:
    //   f (forward) — dirección hacia donde mira la cámara
    //   r (right)   — dirección a la derecha de la cámara
    //   u (up)      — dirección arriba de la cámara
    static Matrix4D lookAt(const Vector3D &eye, const Vector3D &center, const Vector3D &up);

    // Perspective: construye la matriz de proyección en perspectiva.
    //   fov    — campo de visión vertical en radianes
    //   aspect — relación ancho/alto de la ventana
    //   near   — distancia al plano cercano (> 0)
    //   far    — distancia al plano lejano (> near)
    static Matrix4D perspective(float fov, float aspect, float near, float far);

private:
    Vector3D m_target; // Punto al centro donde mira la cámara
    Vector3D m_up;     // Dirección arriba del mundo
    float m_radius;    // Distancia de la cámara al target (radio de la esfera)
    float m_pitch;     // Ángulo vertical: Posición arriba/abajo de la cámara (radianes)
    float m_yaw;       // Ángulo horizontal: Posición izquierda/derecha de la cámara (radianes)
    float m_fov;       // Campo de visión vertical (radianes)
    float m_near;      // Plano cercano
    float m_far;       // Plano lejano
    float m_maxPitch;  // Límite para evitar que la cámara se voltee en el polo

    // Calcula la posición de la cámara a partir de coordenadas esféricas
    Vector3D calculatePosition() const;
};
