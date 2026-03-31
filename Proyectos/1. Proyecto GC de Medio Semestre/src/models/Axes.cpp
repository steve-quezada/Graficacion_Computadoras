// Implementación de los ejes.

#include "models/Axes.h"

Axes::Axes(ShaderProgram *program) : Model(program)
{
    initGeometry();
    initBuffers();
}

void Axes::initGeometry()
{
    // Ejes solo en dirección positiva con letra al final (X, Y, Z).
    const float s = 0.03f; // tamaño de letra

    GLfloat v[] = {
        // Eje X (rojo oscuro)
        0.0f, 0.0f, 0.0f,  0.65f, 0.05f, 0.05f,
        1.0f, 0.0f, 0.0f,  0.65f, 0.05f, 0.05f,

        // Eje Y (verde oscuro)
        0.0f, 0.0f, 0.0f,  0.05f, 0.55f, 0.05f,
        0.0f, 1.0f, 0.0f,  0.05f, 0.55f, 0.05f,
        
        // Eje Z (azul oscuro)
        0.0f, 0.0f, 0.0f,  0.05f, 0.05f, 0.65f,
        0.0f, 0.0f, 1.0f,  0.05f, 0.05f, 0.65f,

        // Letra X (plano YZ)
        // diagonal izquierda a superior derecha
        1.07f, -s, -s,  0.65f, 0.05f, 0.05f,
        1.07f,  s,  s,  0.65f, 0.05f, 0.05f,

        // diagonal derecha a superior izquierda
        1.07f, -s,  s,  0.65f, 0.05f, 0.05f,
        1.07f,  s, -s,  0.65f, 0.05f, 0.05f,

        // Letra Y (plano XZ)
        // diagonal izquierda a superior derecha
        -s, 1.07f,  s,  0.05f, 0.55f, 0.05f,
         0, 1.07f,  0,  0.05f, 0.55f, 0.05f,

        // diagonal derecha a superior izquierda
         s, 1.07f,  s,  0.05f, 0.55f, 0.05f,
         0, 1.07f,  0,  0.05f, 0.55f, 0.05f,

        // recta vertical
         0, 1.07f,  0,  0.05f, 0.55f, 0.05f,
         0, 1.07f, -s,  0.05f, 0.55f, 0.05f,

        // Letra Z (plano XY)
        // recta superior
        -s,  s, 1.07f,  0.05f, 0.05f, 0.65f,
         s,  s, 1.07f,  0.05f, 0.05f, 0.65f,

        // diagonal de izquierda a superior derecha
         s,  s, 1.07f,  0.05f, 0.05f, 0.65f,
        -s, -s, 1.07f,  0.05f, 0.05f, 0.65f,

        // recta inferior
        -s, -s, 1.07f,  0.05f, 0.05f, 0.65f,
         s, -s, 1.07f,  0.05f, 0.05f, 0.65f,
    };
    for (int i = 0; i < 132; i++)
        vertices[i] = v[i];
}

void Axes::initBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo 0: posición (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: color (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Axes::render(const Matrix4D &view, const Matrix4D &projection)
{
    m_shader->use();
    m_shader->setMat4("model", m_model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    glLineWidth(3.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 22);
    glBindVertexArray(0);
    glLineWidth(1.0f);
}

