// Implementación del pipeline de compilación de shaders.

#include "engine/ShaderProgram.h"
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string &vertPath, const std::string &fragPath)
{
    // Vertex Shader
    std::ifstream vertFile(vertPath, std::ios::in);
    if (!vertFile.is_open())
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND (" << vertPath << ")\n";
        return;
    }
    std::string vertSource((std::istreambuf_iterator<char>(vertFile)),
                           std::istreambuf_iterator<char>());
    const char *vertexShaderSource = vertSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment Shader
    std::ifstream fragFile(fragPath, std::ios::in);
    if (!fragFile.is_open())
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND (" << fragPath << ")\n";
        glDeleteShader(vertexShader);
        return;
    }
    std::string fragSource((std::istreambuf_iterator<char>(fragFile)),
                           std::istreambuf_iterator<char>());
    const char *fragmentShaderSource = fragSource.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Link Shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use()
{
    glUseProgram(shaderProgram);
}

void ShaderProgram::setMat4(const std::string &name, const Matrix4D &mat) const
{
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, name.c_str()),
        1, GL_FALSE, mat.value_ptr());
}

void ShaderProgram::setMat3(const std::string &name, const Matrix3D &mat) const
{
    glUniformMatrix3fv(
        glGetUniformLocation(shaderProgram, name.c_str()),
        1, GL_FALSE, &mat.m[0][0]);
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
}

void ShaderProgram::setVec3(const std::string &name, const Vector3D &v) const
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), v.x, v.y, v.z);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void ShaderProgram::setTextureUnit(const std::string &name, int unit) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), unit);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(shaderProgram);
}
