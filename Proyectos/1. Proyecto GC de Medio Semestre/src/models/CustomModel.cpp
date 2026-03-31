// Implementación del modelo cargado desde .obj.

#include "models/CustomModel.h"
#include "loaders/ObjLoader.h"

CustomModel::CustomModel(ShaderProgram *program, const std::string &path)
    : Model(program), m_drawCount{0}
{
    // Extraer el nombre del archivo de la ruta completa
    size_t lastSlash = path.find_last_of("/\\");
    m_name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

    // Cargar el obj y obtener los vértices
    ObjData data = ObjLoader::load(path);

    m_minBound = data.minBound;
    m_maxBound = data.maxBound;
    m_drawCount = static_cast<int>(data.vertices.size()) / 6;

    initBuffers(data.vertices);
}

void CustomModel::initBuffers(const std::vector<float> &vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // Atributo 0: posición (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: color (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void CustomModel::render(const Matrix4D &view, const Matrix4D &projection)
{
    m_shader->use();
    m_shader->setMat4("model", m_model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    glBindVertexArray(0);
}

Vector3D CustomModel::getCenter() const
{
    return (m_minBound + m_maxBound) * 0.5f;
}

float CustomModel::getBoundingRadius() const
{
    Vector3D diff = m_maxBound - m_minBound;
    return diff.length() * 0.5f;
}

std::string CustomModel::getName() const
{
    return m_name;
}
