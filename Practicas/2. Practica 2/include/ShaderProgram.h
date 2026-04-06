// Lee archivos .vert y .frag, los compila en la GPU y los enlaza
#pragma once
#include <GL/glew.h>
#include <string>

class ShaderProgram
{
public:
    unsigned int shaderProgram;

    ShaderProgram(const std::string &vertPath, const std::string &fragPath);
    void use();
    ~ShaderProgram();
};
