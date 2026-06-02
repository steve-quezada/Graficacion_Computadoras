// Implementación del cargador de archivos .obj.

#include "loaders/ObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h> // Para usar memcmp
#include <map>

struct PackedVertex
{
    int v, t, n;

    bool operator<(const PackedVertex &that) const
    {
        if (v != that.v)
            return v < that.v;
        if (t != that.t)
            return t < that.t;
        return n < that.n;
    }
};

static int resolveIndex(int idx, int size)
{
    if (idx > 0)
        return idx - 1;
    else
        return size + idx;
}

ObjLoader::ObjLoader() {}

// Parseo del OBJ
void ObjLoader::parseOBJ(const char *filePath)
{
    // Directorio base
    std::string path(filePath);
    std::string basePath;
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos)
        basePath = path.substr(0, lastSlash + 1);

    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream)
    {
        std::cerr << "ERROR: No se pudo abrir el archivo OBJ: " << filePath << std::endl;
        return;
    }

    // La llave es el objeto PackedVertex y el valor rl indice
    std::map<PackedVertex, unsigned int> vertexToOutIndex;

    auto addVertex = [&](const PackedVertex &packed) -> unsigned int
    {
        auto it = vertexToOutIndex.find(packed);
        if (it != vertexToOutIndex.end())
            return it->second;

        // La posicion
        triangleVerts.push_back(vertVals[packed.v * 3]);
        triangleVerts.push_back(vertVals[packed.v * 3 + 1]);
        triangleVerts.push_back(vertVals[packed.v * 3 + 2]);

        // Textura
        if (packed.t >= 0)
        {
            textureCoords.push_back(stVals[packed.t * 2]);
            textureCoords.push_back(stVals[packed.t * 2 + 1]);
        }
        else
        {
            textureCoords.push_back(0.0f);
            textureCoords.push_back(0.0f);
        }

        // Normales
        if (packed.n >= 0)
        {
            normals.push_back(normVals[packed.n * 3]);
            normals.push_back(normVals[packed.n * 3 + 1]);
            normals.push_back(normVals[packed.n * 3 + 2]);
        }
        else
        {
            normals.push_back(0.0f);
            normals.push_back(1.0f);
            normals.push_back(0.0f);
        }

        unsigned int newIndex = (unsigned int)(triangleVerts.size() / 3) - 1;
        vertexToOutIndex[packed] = newIndex;
        return newIndex;
    };

    std::string line;
    while (std::getline(fileStream, line))
    {
        if (line.compare(0, 2, "v ") == 0)
        {
            float x, y, z;
            std::stringstream ss(line.substr(2));
            ss >> x >> y >> z;
            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);
        }
        else if (line.compare(0, 3, "vt ") == 0)
        {
            float u, v;
            std::stringstream ss(line.substr(3));
            ss >> u >> v;
            stVals.push_back(u);
            stVals.push_back(v);
        }
        else if (line.compare(0, 3, "vn ") == 0)
        {
            float nx, ny, nz;
            std::stringstream ss(line.substr(3));
            ss >> nx >> ny >> nz;
            normVals.push_back(nx);
            normVals.push_back(ny);
            normVals.push_back(nz);
        }
        else if (line.compare(0, 2, "f ") == 0)
        {
            std::stringstream ss(line.substr(2));
            std::string oneCorner;
            std::vector<PackedVertex> faceVerts; // guarda los vértices de la cara

            while (ss >> oneCorner)
            {
                std::string v_str, t_str, n_str;
                std::stringstream cornerSS(oneCorner);
                std::getline(cornerSS, v_str, '/');
                std::getline(cornerSS, t_str, '/');
                std::getline(cornerSS, n_str, '/');

                PackedVertex packed;
                packed.v = resolveIndex(std::stoi(v_str), (int)vertVals.size() / 3);
                packed.t = !t_str.empty() ? resolveIndex(std::stoi(t_str), (int)stVals.size() / 2) : -1;
                packed.n = !n_str.empty() ? resolveIndex(std::stoi(n_str), (int)normVals.size() / 3) : -1;

                faceVerts.push_back(packed);
            }

            if (faceVerts.size() == 3)
            {
                // Triángulo directo
                for (const auto &pv : faceVerts)
                    indices.push_back(addVertex(pv));
            }
            else if (faceVerts.size() > 3)
            {

                for (size_t i = 1; i < faceVerts.size() - 1; ++i)
                {
                    indices.push_back(addVertex(faceVerts[0]));
                    indices.push_back(addVertex(faceVerts[i]));
                    indices.push_back(addVertex(faceVerts[i + 1]));
                }
            }
            // Si es línea (2 vértices) o punto (1 vértice) se ignora
        }
    }
}

// getters
int ObjLoader::getNumVertices() { return (int)(triangleVerts.size() / 3); }
std::vector<float> ObjLoader::getVertices() { return triangleVerts; }
std::vector<float> ObjLoader::getTextureCoordinates() { return textureCoords; }
std::vector<float> ObjLoader::getNormals() { return normals; }
std::vector<unsigned int> ObjLoader::getIndices() { return indices; }
