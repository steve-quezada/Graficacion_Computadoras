// Implementación del modelo cargado desde .obj.

#include "models/CustomModel.h"
#include "loaders/ObjLoader.h"
#ifdef HAVE_SOIL2
#include <soil2/SOIL2.h>
#endif
#include <cmath>
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
    if (m_textureID)    glDeleteTextures(1, &m_textureID);
    if (m_normalMapID)  glDeleteTextures(1, &m_normalMapID);
    if (m_normalVAO)    glDeleteVertexArrays(1, &m_normalVAO);
    if (m_tangentVBO)   glDeleteBuffers(1, &m_tangentVBO);
    if (m_bitangentVBO) glDeleteBuffers(1, &m_bitangentVBO);
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

// Normal Mapping

// Calcula tangentes y bitangentes por triángulo y las promedia por vértice.
void CustomModel::computeTangents()
{
    int numVerts = static_cast<int>(m_vertices.size()) / 3;
    m_tangents.assign(numVerts * 3, 0.0f);
    m_bitangents.assign(numVerts * 3, 0.0f);

    for (int i = 0; i + 2 < static_cast<int>(m_indices.size()); i += 3)
    {
        unsigned int i0 = m_indices[i], i1 = m_indices[i + 1], i2 = m_indices[i + 2];

        // Posiciones
        float p0[3] = {m_vertices[i0*3], m_vertices[i0*3+1], m_vertices[i0*3+2]};
        float p1[3] = {m_vertices[i1*3], m_vertices[i1*3+1], m_vertices[i1*3+2]};
        float p2[3] = {m_vertices[i2*3], m_vertices[i2*3+1], m_vertices[i2*3+2]};

        // UVs
        float u0[2] = {m_textCoords[i0*2], m_textCoords[i0*2+1]};
        float u1[2] = {m_textCoords[i1*2], m_textCoords[i1*2+1]};
        float u2[2] = {m_textCoords[i2*2], m_textCoords[i2*2+1]};

        float e1[3] = {p1[0]-p0[0], p1[1]-p0[1], p1[2]-p0[2]};
        float e2[3] = {p2[0]-p0[0], p2[1]-p0[1], p2[2]-p0[2]};

        float d1[2] = {u1[0]-u0[0], u1[1]-u0[1]};
        float d2[2] = {u2[0]-u0[0], u2[1]-u0[1]};

        float det = d1[0]*d2[1] - d2[0]*d1[1];
        float f   = (std::fabs(det) > 1e-8f) ? 1.0f / det : 1.0f;

        float T[3] = {
            f * (d2[1]*e1[0] - d1[1]*e2[0]),
            f * (d2[1]*e1[1] - d1[1]*e2[1]),
            f * (d2[1]*e1[2] - d1[1]*e2[2])
        };
        float B[3] = {
            f * (-d2[0]*e1[0] + d1[0]*e2[0]),
            f * (-d2[0]*e1[1] + d1[0]*e2[1]),
            f * (-d2[0]*e1[2] + d1[0]*e2[2])
        };

        for (int j = 0; j < 3; ++j)
        {
            unsigned int idx = m_indices[i + j];
            m_tangents[idx*3]     += T[0];
            m_tangents[idx*3 + 1] += T[1];
            m_tangents[idx*3 + 2] += T[2];
            m_bitangents[idx*3]     += B[0];
            m_bitangents[idx*3 + 1] += B[1];
            m_bitangents[idx*3 + 2] += B[2];
        }
    }

    // Normalizar
    for (int i = 0; i < numVerts; ++i)
    {
        auto norm3 = [](float *v){
            float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            if (len > 1e-8f) { v[0]/=len; v[1]/=len; v[2]/=len; }
        };
        norm3(&m_tangents[i*3]);
        norm3(&m_bitangents[i*3]);
    }
}

// Crea un segundo VAO con: location 0 = pos, 1 = normal, 2 = texcoord, 3 = tangent, 4 = bitangent
void CustomModel::initNormalVAO()
{
    // Crear VBOs para tangentes y bitangentes
    glGenBuffers(1, &m_tangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 m_tangents.size() * sizeof(float),
                 m_tangents.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_bitangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_bitangentVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 m_bitangents.size() * sizeof(float),
                 m_bitangents.data(), GL_STATIC_DRAW);

    // Crear VAO para el normal shader
    glGenVertexArrays(1, &m_normalVAO);
    glBindVertexArray(m_normalVAO);

    // loc 0: posiciones
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // loc 1: normales
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // loc 2: texcoords
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // loc 3: tangentes
    glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);

    // loc 4: bitangentes
    glBindBuffer(GL_ARRAY_BUFFER, m_bitangentVBO);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);

    // EBO compartido con el VAO principal
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glBindVertexArray(0);
}

// Carga el mapa de normales y prepara el VAO de normal mapping.
void CustomModel::loadNormalMap(const char *path)
{
#ifdef HAVE_SOIL2
    m_normalMapID = SOIL_load_OGL_texture(
        path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        0);

    if (m_normalMapID == 0)
    {
        std::cerr << "  [NormalMap] No se pudo cargar: " << path << "\n";
        return;
    }
    std::cout << "  [NormalMap] Cargado: " << path << "\n\n";
    computeTangents();
    initNormalVAO();
#endif
}

// Renderiza con el shader de normal mapping.
void CustomModel::renderNormal(ShaderProgram *normalShader, const Matrix4D &view, const Matrix4D &projection)
{
    if (m_normalVAO == 0 || m_normalMapID == 0)
    {
        // Fallback al shader principal
        render(view, projection);
        return;
    }

    normalShader->use();
    normalShader->setMat4("model",      m_model);
    normalShader->setMat4("view",       view);
    normalShader->setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    normalShader->setTextureUnit("samp", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMapID);
    normalShader->setTextureUnit("normalMap", 1);

    glBindVertexArray(m_normalVAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
