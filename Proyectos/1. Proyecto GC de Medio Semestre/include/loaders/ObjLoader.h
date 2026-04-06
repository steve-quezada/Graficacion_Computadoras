// Carga un archivo .obj y almacena los vértices (posición + color por cara).

#pragma once
#include <string>
#include <vector>
#include "math/Vector3D.h"

struct ObjData
{
    std::vector<float> vertices; // posición (3) + color (3) por vértice
    int vertexCount;             // vértices originales
    int faceCount;               // caras del archivo
    Vector3D minBound;           // esquina mínima del bounding box
    Vector3D maxBound;           // esquina máxima del bounding box
};

class ObjLoader
{
public:
    // Lee un archivo .obj y devuelve los datos con colores por cara
    static ObjData load(const std::string &path);
};
