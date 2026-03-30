// Implementación de la clase base.

#include "models/Model.h"

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
