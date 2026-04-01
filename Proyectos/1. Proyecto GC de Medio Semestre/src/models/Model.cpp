// Implementación de la clase base.

#include "models/Model.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Model::rotate(float degrees, char axis)
{
    float rad = degrees * static_cast<float>(M_PI) / 180.0f;
    if (axis == 'x' || axis == 'X')
        m_model = m_model * Matrix4D::rotateX(rad);
    else if (axis == 'y' || axis == 'Y')
        m_model = m_model * Matrix4D::rotateY(rad);
    else if (axis == 'z' || axis == 'Z')
        m_model = m_model * Matrix4D::rotateZ(rad);
}

void Model::translate(float dx, float dy, float dz)
{
    m_model = Matrix4D::translate({dx, dy, dz}) * m_model;
}

void Model::scale(float factor)
{
    m_model = m_model * Matrix4D::scale({factor, factor, factor});
}

void Model::resetTransform()
{
    m_model = Matrix4D();
}
