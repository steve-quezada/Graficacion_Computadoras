// Implementación de la Cámara FPS (primera persona)

#include "engine/Camera.h"
#include <cmath>

Camera::Camera(const Vector3D &target, float radius, float pitch, float yaw,
               float fov, float near, float far)
    : m_target{target},
      m_up{0.0f, 1.0f, 0.0f},
      m_radius{radius},
      m_pitch{pitch},
      m_yaw{yaw},
      m_fov{fov},
      m_near{near},
      m_far{far},
      m_maxPitch{1.55f},
      m_freeMode{false},
      m_freePos{0.0f, 0.0f, 0.0f}
{
}

void Camera::orbit(float deltaYaw, float deltaPitch)
{
    // El yaw rota libremente alrededor del target
    m_yaw += deltaYaw;

    // El pitch se limita para que la cámara no cruce el polo
    m_pitch += deltaPitch;
    if (m_pitch > m_maxPitch)
        m_pitch = m_maxPitch;
    if (m_pitch < -m_maxPitch)
        m_pitch = -m_maxPitch;
}

void Camera::zoom(float amount)
{
    m_radius += amount;
    if (m_radius < 0.1f)
        m_radius = 0.1f;
}

void Camera::clampOrbitalMinY(float minY)
{
    if (m_freeMode || m_radius <= 0.0f)
        return;

    float ratio = (minY - m_target.y) / m_radius;
    if (ratio <= -1.0f)
        return;

    if (ratio >= 1.0f)
    {
        m_pitch = m_maxPitch;
        return;
    }

    float minPitch = std::asin(ratio);
    if (m_pitch < minPitch)
        m_pitch = minPitch;
}

void Camera::resetView(const Vector3D &target, float radius)
{
    m_target = target;
    m_radius = radius;
    m_pitch = 0.5f;
    m_yaw = 0.0f;
    m_freeMode = false;
}

Matrix4D Camera::getViewMatrix() const
{
    if (m_freeMode)
    {
        Vector3D center = m_freePos + getFront();
        return lookAt(m_freePos, center, m_up);
    }
    Vector3D pos = calculatePosition();
    return lookAt(pos, m_target, m_up);
}

Vector3D Camera::getPosition() const
{
    if (m_freeMode)
        return m_freePos;
    return calculatePosition();
}

Vector3D Camera::getFront() const
{
    return Vector3D{
        -std::cos(m_pitch) * std::sin(m_yaw),
        -std::sin(m_pitch),
        -std::cos(m_pitch) * std::cos(m_yaw)};
}

Vector3D Camera::getRight() const
{
    return getFront().cross(m_up).normalized();
}

void Camera::toggleFreeMode()
{
    m_freeMode = !m_freeMode;
    if (m_freeMode)
        m_freePos = calculatePosition();
}

bool Camera::isFreeMode() const
{
    return m_freeMode;
}

void Camera::move(float forward, float right, float up)
{
    Vector3D fwd = getFront();
    Vector3D r = fwd.cross(m_up).normalized();
    m_freePos += fwd * forward + r * right + m_up * up;
}

void Camera::setPosition(const Vector3D &pos)
{
    m_freePos = pos;
    m_freeMode = true;
}

Matrix4D Camera::getProjectionMatrix(float aspect) const
{
    return perspective(m_fov, aspect, m_near, m_far);
}

Vector3D Camera::calculatePosition() const
{
    // Conversión de coordenadas esféricas (radio, pitch, yaw) a cartesianas (x, y, z).
    // La cámara está a m_radius unidades del target.
    //   x = r * cos(pitch) * sin(yaw)  — componente horizontal
    //   y = r * sin(pitch)             — componente vertical
    //   z = r * cos(pitch) * cos(yaw)  — componente de profundidad
    // Se suma m_target para que la cámara orbite alrededor del target, no del origen
    return {
        m_target.x + m_radius * std::cos(m_pitch) * std::sin(m_yaw),
        m_target.y + m_radius * std::sin(m_pitch),
        m_target.z + m_radius * std::cos(m_pitch) * std::cos(m_yaw)};
}

Matrix4D Camera::lookAt(const Vector3D &eye, const Vector3D &center, const Vector3D &up)
{
    // Calcula los 3 ejes ortogonales del espacio de cámara
    Vector3D f = (center - eye).normalized();
    Vector3D r = f.cross(up).normalized();
    Vector3D u = r.cross(f);

    // La matriz de rotación convierte coordenadas del mundo al espacio de cámara.
    // Cada fila es uno de los ejes: r en fila 0, u en fila 1, -f en fila 2.
    // Se niega f porque OpenGL mira hacia -Z (la cámara apunta en sentido negativo).
    Matrix4D rot;
    rot(0, 0) = r.x;
    rot(0, 1) = r.y;
    rot(0, 2) = r.z;
    rot(1, 0) = u.x;
    rot(1, 1) = u.y;
    rot(1, 2) = u.z;
    rot(2, 0) = -f.x;
    rot(2, 1) = -f.y;
    rot(2, 2) = -f.z;

    // La traslación mueve el mundo para que la cámara quede en el origen
    Matrix4D trans = Matrix4D::translate({-eye.x, -eye.y, -eye.z});

    return rot * trans;
}

Matrix4D Camera::perspective(float fov, float aspect, float near, float far)
{
    // Tangente del angulo vertical define cuánto se ve arriba/abajo
    float tanHalf = std::tan(fov / 2.0f);

    // Matriz identidad con (3,3) = 0 para la división homogénea
    Matrix4D mat;
    mat(3, 3) = 0.0f;

    // Escala X e Y según el ángulo de visión y aspecto
    mat(0, 0) = 1.0f / (aspect * tanHalf);
    mat(1, 1) = 1.0f / tanHalf;

    // Mapea Z del rango [near, far] al rango [-1, 1]
    mat(2, 2) = -(far + near) / (far - near);        // factor de profundidad
    mat(2, 3) = -(2.0f * far * near) / (far - near); // offset de profundidad

    // Copia -Z en la componente W del vector de salida.
    mat(3, 2) = -1.0f;

    return mat;
}
