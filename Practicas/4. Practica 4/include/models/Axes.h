// Ejes (X,Y,Z).

#pragma once
#include "models/Model.h"

class Axes : public Model
{
public:
    // Crea 3 líneas de colores para los ejes 
    Axes(ShaderProgram *program);

    // Dibuja los ejes con GL_LINES
    void render(const Matrix4D &view, const Matrix4D &projection) override;

private:
    GLfloat vertices[132]; // 22 vértices × (3 pos + 3 color)

    // Define los datos de posición y color
    void initGeometry();

    // Crea VAO y VBO en la GPU
    void initBuffers();
};
