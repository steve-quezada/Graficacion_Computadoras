// Lee, compila y enlaza un vertex y fragment shader en la GPU.

#pragma once
#include <GL/glew.h>
#include <string>
#include "math/Matrix4D.h"

class ShaderProgram
{
public:
    unsigned int shaderProgram;

    // Lee los archivos .vert y .frag, compila y enlaza el programa en la GPU
    ShaderProgram(const std::string &vertPath, const std::string &fragPath);

    // Activa este shader program para el siguiente draw call
    void use();
    
    // Envía una matriz 4×4 al shader
    void setMat4(const std::string &name, const Matrix4D &mat) const;

    // Elimina el programa de la GPU
    ~ShaderProgram();
};
