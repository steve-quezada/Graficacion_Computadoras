// Guarda los vértices en la GPU [VAO + VBO] y los dibuja

#pragma once
#include <GL/glew.h>

class Polygon {
public:
    Polygon(const float* vertices, int vertexCount); // Crea VAO y VBO
    void draw();
    ~Polygon();

private:
    unsigned int VAO;
    unsigned int VBO;
    int vertexCount;
};
