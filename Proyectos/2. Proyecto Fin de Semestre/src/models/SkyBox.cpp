#include "models/SkyBox.h"

#ifdef HAVE_SOIL2
#include <soil2/SOIL2.h>
#endif

#include <cstdlib>  // free()
#include <iostream>
#include <string>

// 36 vértices (12 triángulos) para un cubo centrado en el origen
static const float SKY_VERTS[] = {
    // +X (right)
    -1.f,  1.f, -1.f,   -1.f, -1.f, -1.f,    1.f, -1.f, -1.f,
     1.f, -1.f, -1.f,    1.f,  1.f, -1.f,   -1.f,  1.f, -1.f,
    // -X (left)
    -1.f, -1.f,  1.f,   -1.f, -1.f, -1.f,   -1.f,  1.f, -1.f,
    -1.f,  1.f, -1.f,   -1.f,  1.f,  1.f,   -1.f, -1.f,  1.f,
    // +Y (top)
     1.f, -1.f, -1.f,    1.f, -1.f,  1.f,    1.f,  1.f,  1.f,
     1.f,  1.f,  1.f,    1.f,  1.f, -1.f,    1.f, -1.f, -1.f,
    // -Y (bottom)
    -1.f, -1.f,  1.f,   -1.f,  1.f,  1.f,    1.f,  1.f,  1.f,
     1.f,  1.f,  1.f,    1.f, -1.f,  1.f,   -1.f, -1.f,  1.f,
    // +Z (front)
    -1.f,  1.f, -1.f,    1.f,  1.f, -1.f,    1.f,  1.f,  1.f,
     1.f,  1.f,  1.f,   -1.f,  1.f,  1.f,   -1.f,  1.f, -1.f,
    // -Z (back)
    -1.f, -1.f, -1.f,   -1.f, -1.f,  1.f,    1.f, -1.f, -1.f,
     1.f, -1.f, -1.f,   -1.f, -1.f,  1.f,    1.f, -1.f,  1.f
};

SkyBox::SkyBox(ShaderProgram *shader, const char *facesDir)
    : m_shader(shader)
{
    setupGeometry();
    m_cubemapID = loadCubemap(facesDir);
}

SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteTextures(1, &m_cubemapID);
}

void SkyBox::setupGeometry()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SKY_VERTS), SKY_VERTS, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

GLuint SkyBox::loadCubemap(const char *dir)
{
    const char *faces[6] = { "right.jpg", "left.jpg", "top.jpg",
                              "bottom.jpg", "front.jpg", "back.jpg" };
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    for (int i = 0; i < 6; ++i)
    {
        std::string path = std::string(dir) + faces[i];

#ifdef HAVE_SOIL2
        int w, h, ch;
        unsigned char *data = SOIL_load_image(path.c_str(), &w, &h, &ch, SOIL_LOAD_RGB);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            free(data);
        }
        else
        {
            std::cerr << "[SkyBox] No se pudo cargar: " << path << "\n";
        }
#else
        std::cerr << "[SkyBox] SOIL2 no disponible, no se carga: " << path << "\n";
#endif
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return id;
}

void SkyBox::render(const Matrix4D &view, const Matrix4D &projection)
{
    // Quitar la traslación de la vista (solo queremos rotación)
    Matrix4D viewNoTrans = view;
    viewNoTrans.m[3][0] = 0.f;
    viewNoTrans.m[3][1] = 0.f;
    viewNoTrans.m[3][2] = 0.f;

    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    m_shader->use();
    m_shader->setMat4("view",       viewNoTrans);
    m_shader->setMat4("projection", projection);
    m_shader->setInt("skybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapID);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
