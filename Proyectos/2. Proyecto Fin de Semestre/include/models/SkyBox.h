#pragma once
#include <GL/glew.h>
#include "engine/ShaderProgram.h"
#include "math/Matrix4D.h"

// Skybox usando un cubemap de 6 imágenes.
// Las caras se deben llamar: right, left, top, bottom, front, back con extensión .jpg.
class SkyBox
{
public:
    // Directorio que contiene las 6 imágenes.
    explicit SkyBox(ShaderProgram *shader, const char *facesDir = "assets/skybox/");
    ~SkyBox();

    // Se debe llamar antes de dibujar los objetos opacos.
    void render(const Matrix4D &view, const Matrix4D &projection);

private:
    ShaderProgram *m_shader{nullptr}; // Shader para skybox
    GLuint m_vao{0}, m_vbo{0};        // Geometría de un cubo centrado en el origen
    GLuint m_cubemapID{0};            // ID de la textura cubemap

    // Carga la geometría del cubo y la sube a la GPU
    void setupGeometry();

    // Carga las 6 imágenes del cubemap y crea la textura en la GPU
    GLuint loadCubemap(const char *dir);
};
