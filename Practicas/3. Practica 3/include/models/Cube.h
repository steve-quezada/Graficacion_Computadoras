// Hereda de Model. Define un cubo unitario centrado en el origen.

#pragma once
#include "models/Model.h"

class Cube : public Model
{
public:
    // Configura la geometría del cubo y carga los buffers en la GPU
    Cube(ShaderProgram *program);

    // Dibuja el cubo con las matrices view y projection
    void render(const Matrix4D &view, const Matrix4D &projection) override;

    // Aplica rotación continua en Y usando delta time
    void update(float deltaTime) override;

private:
    GLfloat vertices[48];  // 8 vértices × (3 pos + 3 color)
    GLuint indices[36];    // 12 triángulos × 3 índices

    // Define los datos de posición y color de los 8 vértices
    void initGeometry();

    // Crea VAO, VBO y EBO en la GPU y configura los atributos de vértice
    void initBuffers();
};
