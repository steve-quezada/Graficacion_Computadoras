// Clase base abstracta para cualquier modelo 3D.
// Administra VAO, VBO, EBO y mantiene la matriz de modelo.

#pragma once
#include <cmath>
#include <GL/glew.h>
#include "engine/ShaderProgram.h"
#include "math/Matrix4D.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Model
{
public:
    // Asocia el shader al modelo e inicializa los IDs de buffer a 0
    Model(ShaderProgram *program)
        : m_shader{program}, VAO{0}, VBO{0}, EBO{0} {}

    // Libera VAO, VBO y EBO de la GPU
    virtual ~Model();

    // Dibuja el modelo usando las matrices de vista y proyección
    virtual void render(const Matrix4D &view, const Matrix4D &projection) = 0;

    // Actualiza la lógica del modelo con el delta de tiempo
    virtual void update(float deltaTime) = 0;

    // Rota la matriz del modelo en el eje dado (ángulo en grados)
    void rotate(float degrees, char axis);

protected:
    ShaderProgram *m_shader;
    GLuint VAO, VBO, EBO;
    Matrix4D m_model; // Matriz de modelo
};
