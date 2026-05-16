// Implementación del modelo cargado desde .obj.

#include "models/CustomModel.h"
#include "loaders/ObjLoader.h"
#ifdef HAVE_SOIL2
#include <SOIL2/SOIL2.h>
#endif
#include <iostream>

CustomModel::CustomModel(ShaderProgram *program, const char *filePath, const char *texturePath)
    : Model(program), m_filePath(filePath), m_numIndices(0), m_textureID(0)
{
    // Extraer el nombre del archivo
    std::string path(filePath);
    size_t lastSlash = path.find_last_of("/\\");
    m_name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;

    initGeometry();
    init();

    // Cargar textura con la ruta
    if (texturePath[0] != '\0')
        m_textureID = loadTexture(texturePath);

    if (m_textureID == 0)
        m_textureID = createWhiteTexture();
}

CustomModel::~CustomModel()
{
    if (m_textureID)
        glDeleteTextures(1, &m_textureID);
    glDeleteBuffers(NUM_VBOS, m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

/**
 * @brief Carga los datos desde el archivo OBJ y los prepara para la indexación.
 */
void CustomModel::initGeometry()
{
    ObjLoader obj_loader;
    obj_loader.parseOBJ(m_filePath);

    // Obtenemos los vectores procesados por el loader (ya sin duplicados)
    m_vertices = obj_loader.getVertices();
    m_textCoords = obj_loader.getTextureCoordinates();
    m_normalVecs = obj_loader.getNormals();
    m_indices = obj_loader.getIndices();

    m_numIndices = static_cast<int>(m_indices.size());

    // Bounding box a partir de las posiciones cargadas
    if (!m_vertices.empty())
    {
        m_minBound = {m_vertices[0], m_vertices[1], m_vertices[2]};
        m_maxBound = {m_vertices[0], m_vertices[1], m_vertices[2]};
        for (int i = 0; i + 2 < (int)m_vertices.size(); i += 3)
        {
            if (m_vertices[i] < m_minBound.x)
                m_minBound.x = m_vertices[i];
            if (m_vertices[i + 1] < m_minBound.y)
                m_minBound.y = m_vertices[i + 1];
            if (m_vertices[i + 2] < m_minBound.z)
                m_minBound.z = m_vertices[i + 2];
            if (m_vertices[i] > m_maxBound.x)
                m_maxBound.x = m_vertices[i];
            if (m_vertices[i + 1] > m_maxBound.y)
                m_maxBound.y = m_vertices[i + 1];
            if (m_vertices[i + 2] > m_maxBound.z)
                m_maxBound.z = m_vertices[i + 2];
        }
    }

    std::cout << "  OBJ cargado: " << m_name
              << " | vertices=" << obj_loader.getNumVertices()
              << " | indices=" << m_numIndices << "\n";
}

/**
 * @brief Configura el VAO, VBOs y el EBO en la GPU.
 */
void CustomModel::init()
{
    // 1. Generar objetos
    glGenVertexArrays(1, &VAO);
    glGenBuffers(NUM_VBOS, m_VBO);
    glGenBuffers(1, &m_EBO);

    // 2. Atar el VAO para empezar a grabar la configuración
    glBindVertexArray(VAO);

    // --- POSICIONES (Atributo 0) ---
    if (!m_vertices.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
    }

    // --- TEXTURAS (Atributo 1) ---
    if (!m_textCoords.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, m_textCoords.size() * sizeof(float), m_textCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(1);
    }

    // --- NORMALES (Atributo 2) ---
    if (!m_normalVecs.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, m_normalVecs.size() * sizeof(float), m_normalVecs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(2);
    }

    // --- ELEMENT BUFFER (EBO) ---
    if (!m_indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
    }
    else
    {
        std::cerr << "CUSTOM_MODEL_ERROR: No se pudieron cargar los índices para: " << m_filePath << std::endl;
    }

    // Desatamos el VAO
    glBindVertexArray(0);
}

/**
 * @brief Renderiza el modelo usando glDrawElements para optimizar el caché de vértices.
 */
void CustomModel::render(const Matrix4D &view, const Matrix4D &projection)
{
    if (m_numIndices == 0)
        return;

    m_shader->use();

    renderTexture();

    // Pasar matrices de transformación al shader
    m_shader->setMat4("model", m_model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    // Al usar VAOs configurados en el init, solo bindeamos y dibujamos
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CustomModel::renderTexture()
{
    glActiveTexture(GL_TEXTURE0);              // Activamos unidad de textura 0
    glBindTexture(GL_TEXTURE_2D, m_textureID); // asignamos una textura a esa unidad
    
    // Le pasamos 0 por que es la unidad de textura que definimos en el fragment shader
    m_shader->setTextureUnit("samp", 0);
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

void CustomModel::normalize()
{
    Vector3D center = getCenter();
    float radius = getBoundingRadius();
    if (radius < 1e-6f)
        radius = 1.0f;

    m_model = Matrix4D::scale({1.0f / radius, 1.0f / radius, 1.0f / radius}) * Matrix4D::translate({-center.x, -center.y, -center.z});
}

// Textura
GLuint CustomModel::loadTexture(const char *texPath)
{
#ifdef HAVE_SOIL2
    GLuint id = SOIL_load_OGL_texture(
        texPath,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y);

    if (id == 0)
        std::cerr << "  SOIL: no se pudo cargar la textura: " << texPath << std::endl;
    else
        std::cout << "  SOIL: textura cargada: " << texPath << std::endl
                  << std::endl;

    return id;
#else
    (void)texPath;
    return 0;
#endif
}

GLuint CustomModel::createWhiteTexture()
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    unsigned char white[4] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return id;
}
