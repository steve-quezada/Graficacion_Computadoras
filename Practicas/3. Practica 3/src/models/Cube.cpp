// Implementación de la geometría del cubo.

#include "models/Cube.h"

Cube::Cube(ShaderProgram *program) : Model(program)
{
    initGeometry();
    initBuffers();
}

void Cube::initGeometry()
{
    // 8 vértices: posición (x,y,z) + color (r,g,b)
    GLfloat v[] = {
        // posición           // color
        -0.5f, -0.5f, -0.5f,  0.00f, 0.10f, 0.10f,
         0.5f, -0.5f, -0.5f,  0.00f, 0.25f, 0.25f,
         0.5f,  0.5f, -0.5f,  0.00f, 0.45f, 0.43f,
        -0.5f,  0.5f, -0.5f,  0.00f, 0.35f, 0.33f,

        // Cara frontal z = 0.5
        -0.5f, -0.5f, 0.5f,   0.00f, 0.60f, 0.58f,
         0.5f, -0.5f, 0.5f,   0.00f, 0.75f, 0.72f,
         0.5f,  0.5f, 0.5f,   0.00f, 0.90f, 0.87f,
        -0.5f,  0.5f, 0.5f,   0.00f, 1.00f, 0.95f
    };
    // En lugar de asignar cada índice por separado, se agrupa en un arreglo temporal 
    // y se copia con un ciclo
    for (int i = 0; i < 48; i++)
        vertices[i] = v[i];

    // 6 caras × 2 triángulos = 36 índices
    GLuint idx[] = {
        // Trasera
        0, 2, 1, 3, 2, 0,
        // Frontal
        4, 5, 6, 6, 7, 4,
        // Inferior
        0, 1, 5, 5, 4, 0,
        // Superior
        2, 3, 7, 7, 6, 2,
        // Izquierda
        0, 7, 3, 7, 0, 4,
        // Derecha
        1, 2, 6, 6, 5, 1};
    for (int i = 0; i < 36; i++)
        indices[i] = idx[i];
}

void Cube::initBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atributo 0: posición (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: color (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cube::render(const Matrix4D &view, const Matrix4D &projection)
{
    // Activa el shader y envía las 3 matrices
    m_shader->use();
    m_shader->setMat4("model", m_model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    // Dibuja los 36 índices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::update(float deltaTime)
{
    // Rotar 45 grados por segundo en el eje Y usando la función rotate del Model
    rotate(45.0f * deltaTime, 'Y');
}
