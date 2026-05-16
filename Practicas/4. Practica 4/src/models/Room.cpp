// Implementación de la Habitación explorable

#include "models/Room.h"
#include <SOIL2/SOIL2.h>
#include <vector>

Room::Room(ShaderProgram *program, float halfSize)
    : Model(program), m_indexCount{0}, m_texFloor{0}, m_texCeil{0}, m_texWall{0}, m_halfSize{halfSize}
{
    initGeometry(halfSize);

    m_texFloor = SOIL_load_OGL_texture("assets/textures/room/floor.jpg",
                                       SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    m_texCeil = SOIL_load_OGL_texture("assets/textures/room/ceil.jpg",
                                      SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    m_texWall = SOIL_load_OGL_texture("assets/textures/room/wall.jpg",
                                      SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
}

Room::~Room()
{
    if (m_texFloor)
        glDeleteTextures(1, &m_texFloor);
    if (m_texCeil)
        glDeleteTextures(1, &m_texCeil);
    if (m_texWall)
        glDeleteTextures(1, &m_texWall);
}

// Geometría del cubo
void Room::initGeometry(float h)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Piso +Y
    vertices.insert(vertices.end(), {-h, -h, -h, 0, 1, 0, 0, 0,
                                     h, -h, -h, 0, 1, 0, 1, 0,
                                     h, -h, h, 0, 1, 0, 1, 1,
                                     -h, -h, h, 0, 1, 0, 0, 1});
    indices.insert(indices.end(), {0, 1, 2, 0, 2, 3});

    // Techo -Y
    vertices.insert(vertices.end(), {-h, h, h, 0, -1, 0, 0, 0,
                                     h, h, h, 0, -1, 0, 1, 0,
                                     h, h, -h, 0, -1, 0, 1, 1,
                                     -h, h, -h, 0, -1, 0, 0, 1});
    indices.insert(indices.end(), {4, 5, 6, 4, 6, 7});

    // Pared frontal -Z
    vertices.insert(vertices.end(), {-h, -h, h, 0, 0, -1, 0, 0,
                                     h, -h, h, 0, 0, -1, 1, 0,
                                     h, h, h, 0, 0, -1, 1, 1,
                                     -h, h, h, 0, 0, -1, 0, 1});
    indices.insert(indices.end(), {8, 9, 10, 8, 10, 11});

    // Pared trasera +Z
    vertices.insert(vertices.end(), {h, -h, -h, 0, 0, 1, 0, 0,
                                     -h, -h, -h, 0, 0, 1, 1, 0,
                                     -h, h, -h, 0, 0, 1, 1, 1,
                                     h, h, -h, 0, 0, 1, 0, 1});
    indices.insert(indices.end(), {12, 13, 14, 12, 14, 15});

    // Pared izquierda +X
    vertices.insert(vertices.end(), {-h, -h, -h, 1, 0, 0, 0, 0,
                                     -h, -h, h, 1, 0, 0, 1, 0,
                                     -h, h, h, 1, 0, 0, 1, 1,
                                     -h, h, -h, 1, 0, 0, 0, 1});
    indices.insert(indices.end(), {16, 17, 18, 16, 18, 19});

    // Pared derecha -X
    vertices.insert(vertices.end(), {h, -h, h, -1, 0, 0, 0, 0,
                                     h, -h, -h, -1, 0, 0, 1, 0,
                                     h, h, -h, -1, 0, 0, 1, 1,
                                     h, h, h, -1, 0, 0, 0, 1});
    indices.insert(indices.end(), {20, 21, 22, 20, 22, 23});

    m_indexCount = (int)indices.size();

    // Subir a GPU
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    const GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Room::render(const Matrix4D &view, const Matrix4D &projection)
{
    // Desactivar culling para ver el interior
    glDisable(GL_CULL_FACE);

    m_shader->use();
    m_shader->setMat4("model", m_model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    m_shader->setTextureUnit("samp", 0);
    glBindVertexArray(VAO);

    // Piso (6 índices desde offset 0)
    glBindTexture(GL_TEXTURE_2D, m_texFloor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

    // Techo (6 índices desde offset 6)
    glBindTexture(GL_TEXTURE_2D, m_texCeil);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                   (void *)(6 * sizeof(unsigned int)));

    // Paredes — 4 caras (24 índices desde offset 12)
    glBindTexture(GL_TEXTURE_2D, m_texWall);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT,
                   (void *)(12 * sizeof(unsigned int)));

    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);
}
