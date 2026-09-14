// Lee, compila y enlaza un vertex y fragment shader en la GPU.

#pragma once
#include <GL/glew.h>
#include <string>
#include "math/Matrix4D.h"
#include "math/Matrix3D.h"
#include "math/Vector3D.h"

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

    // Envía una matriz 3×3 al shader
    void setMat3(const std::string &name, const Matrix3D &mat) const;

    // Envía un vec3 al shader
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, const Vector3D &v) const;

    // Envía un float al shader
    void setFloat(const std::string &name, float value) const;

    // Envía un int al shader (también sirve para sampler2D)
    void setInt(const std::string &name, int value) const;

    // Asigna una unidad de textura a un sampler
    void setTextureUnit(const std::string &name, int unit) const;

    // Elimina el programa de la GPU
    ~ShaderProgram();
};
