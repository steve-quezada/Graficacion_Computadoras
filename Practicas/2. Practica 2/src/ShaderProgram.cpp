// Lee archivos .vert y .frag, los compila en la GPU y los enlaza
#include "ShaderProgram.h"
#include <fstream>
#include <iostream>

// Construccion y compilacion del shader program [main.cpp]
ShaderProgram::ShaderProgram(const std::string& vertPath, const std::string& fragPath) {

    // vertex shader
    std::ifstream vertFile(vertPath, std::ios::in);
    if (!vertFile.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND (" << vertPath << ")\n" << std::endl;
        return;
    }
    std::string vertSource((std::istreambuf_iterator<char>(vertFile)),
                            std::istreambuf_iterator<char>());
    const char* vertexShaderSource = vertSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    std::ifstream fragFile(fragPath, std::ios::in);
    if (!fragFile.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND (" << fragPath << ")\n" << std::endl;
        glDeleteShader(vertexShader);
        return;
    }
    std::string fragSource((std::istreambuf_iterator<char>(fragFile)),
                            std::istreambuf_iterator<char>());
    const char* fragmentShaderSource = fragSource.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activar el shader program
void ShaderProgram::use() {
    glUseProgram(shaderProgram);
}

// Libera todos los recursos
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shaderProgram);
}
