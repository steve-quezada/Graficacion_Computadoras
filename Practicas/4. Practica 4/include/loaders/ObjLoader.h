// Cargador de archivos .obj optimizado.

#pragma once
#include <vector>
#include <string>

class ObjLoader
{
private:
    // Valores leidos de un OBJ
    std::vector<float> vertVals;
    std::vector<float> stVals;
    std::vector<float> normVals;

    // Valores almacenados para ser usados como vertex attributes
    std::vector<float> triangleVerts;
    std::vector<float> textureCoords;
    std::vector<float> normals;

    std::vector<unsigned int> indices;

public:
    ObjLoader();

    // Lee el archivo OBJ y llena los vectores
    void parseOBJ(const char *filePath);

    // Getters
    int getNumVertices();
    std::vector<float> getVertices();
    std::vector<float> getTextureCoordinates();
    std::vector<float> getNormals();
    std::vector<unsigned int> getIndices();
};
