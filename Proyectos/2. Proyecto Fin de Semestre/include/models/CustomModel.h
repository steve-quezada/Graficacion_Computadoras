// Modelo cargado desde un archivo .obj. con textura y Phong.

#pragma once
#include "models/Model.h"
#include "math/Vector3D.h"
#include <vector>
#include <string>

// Definimos el número de Vertex Buffer Objects: Posición, Textura, Normales
#define NUM_VBOS 3

class CustomModel : public Model
{
private:
    // Configuración del modelo
    const char *m_filePath;

    // IDs de OpenGL
    GLuint m_VBO[NUM_VBOS]; // VBO[0] = posiciones
                            // VBO[1] = coordenadas de textura
                            // VBO[2] = normales

    GLuint m_EBO; // Element Buffer Object para indexación

    // Datos de la malla en CPU
    std::vector<float> m_vertices;
    std::vector<float> m_textCoords;
    std::vector<float> m_normalVecs;
    std::vector<unsigned int> m_indices; // índices para glDrawElements

    // Contadores para el renderizado
    int m_numIndices;

    // Textura principal
    GLuint m_textureID;

    // Normal Map
    GLuint m_normalVAO{0};           // VAO con layout de normal.vert
    GLuint m_normalMapID{0};         // Textura del mapa de normales
    GLuint m_tangentVBO{0};          // VBO de tangentes calculadas
    GLuint m_bitangentVBO{0};        // VBO de bitangentes calculadas
    std::vector<float> m_tangents;
    std::vector<float> m_bitangents;

    // Bounding box para normalizar la escala
    Vector3D m_minBound;
    Vector3D m_maxBound;

    // Nombre del archivo
    std::string m_name;

    // Extrae los datos del OBJ a los vectores locales
    void initGeometry();

    // Sube los datos a la GPU (VAO principal: pos=0, tex=1, nrm=2)
    void init();

    // Computa tangentes/bitangentes por triángulo
    void computeTangents();

    // Crea m_normalVAO con layout de normal.vert
    void initNormalVAO();

    // Activa y asigna la textura al shader
    void renderTexture();

    // Carga textura con SOIL2
    static GLuint loadTexture(const char *texPath);

    // Textura blanca como fallback
    static GLuint createWhiteTexture();

public:
    // Constructor
    CustomModel(ShaderProgram *program, const char *filePath, const char *texturePath = "");

    ~CustomModel() override;

    // Dibuja el modelo con iluminación Phong + textura
    void render(const Matrix4D &view, const Matrix4D &projection) override;

    // Dibuja el modelo con normal mapping (normal.vert / normal.frag)
    void renderNormal(ShaderProgram *normalShader, const Matrix4D &view, const Matrix4D &projection);

    // Carga el mapa de normales y prepara el VAO para renderizado con normal.vert
    void loadNormalMap(const char *path);

    // Devuelve true si hay un mapa de normales cargado
    bool hasNormalMap() const { return m_normalMapID != 0; }

    // Centro del bounding box en espacio local
    Vector3D getCenter() const;

    // Radio del bounding box
    float getBoundingRadius() const;

    // Nombre del archivo
    std::string getName() const;

    // Escala a radio 1 y centra en el origen
    void normalize();
};
