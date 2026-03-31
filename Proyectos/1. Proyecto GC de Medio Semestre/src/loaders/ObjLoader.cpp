// Implementación del cargador de archivos .obj.

#include "loaders/ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

ObjData ObjLoader::load(const std::string &path)
{
    ObjData data;
    data.vertexCount = 0;
    data.faceCount = 0;

    std::vector<Vector3D> positions;

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "ERROR: No se pudo abrir el archivo OBJ: " << path << std::endl;
        return data;
    }

    // Paleta de colores oscuros (máximo ~0.50f)
    const float colors[][3] = {
        {0.50f, 0.05f, 0.05f}, // rojo
        {0.05f, 0.45f, 0.05f}, // verde
        {0.05f, 0.05f, 0.55f}, // azul
        {0.50f, 0.42f, 0.00f}, // amarillo
        {0.55f, 0.22f, 0.00f}, // naranja
        {0.35f, 0.05f, 0.50f}, // violeta
        {0.00f, 0.40f, 0.40f}, // teal
        {0.50f, 0.05f, 0.32f}, // fucsia
        {0.12f, 0.38f, 0.12f}, // verde medio
        {0.40f, 0.22f, 0.08f}, // marrón
        {0.08f, 0.28f, 0.50f}, // azul acero
        {0.45f, 0.05f, 0.05f}, // rojo ladrillo
        {0.00f, 0.38f, 0.38f}, // cian
        {0.38f, 0.05f, 0.45f}, // púrpura
        {0.20f, 0.05f, 0.38f}, // índigo
        {0.05f, 0.30f, 0.16f}, // verde mar
        {0.55f, 0.18f, 0.02f}, // naranja oscuro
        {0.28f, 0.28f, 0.28f}, // gris
        {0.05f, 0.32f, 0.50f}, // azul cielo
        {0.44f, 0.26f, 0.05f}, // dorado
        {0.12f, 0.42f, 0.22f}, // verde menta
        {0.05f, 0.15f, 0.50f}, // azul real
        {0.45f, 0.05f, 0.22f}, // granate
        {0.00f, 0.34f, 0.22f}, // esmeralda
        {0.38f, 0.12f, 0.00f}, // terracota
        {0.00f, 0.38f, 0.26f}, // verde agua
        {0.34f, 0.26f, 0.00f}, // ocre
        {0.30f, 0.00f, 0.38f}, // violeta oscuro
        {0.44f, 0.08f, 0.28f}, // rosa oscuro
        {0.12f, 0.12f, 0.42f}, // azul pizarra
        {0.34f, 0.16f, 0.34f}, // malva
        {0.20f, 0.38f, 0.05f}, // lima
    };
    const int numColors = 32;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            // Leer posición del vértice
            float x, y, z;
            ss >> x >> y >> z;
            positions.push_back({x, y, z});
        }
        else if (prefix == "f")
        {
            // Leer los índices de la cara
            // Formatos [v, v/vt, v//vn, v/vt/vn]
            std::vector<int> faceIndices;
            std::string token;
            while (ss >> token)
            {
                // Solo interesa el primer número
                int idx = 0;
                size_t slashPos = token.find('/');
                if (slashPos != std::string::npos)
                    idx = std::stoi(token.substr(0, slashPos));
                else
                    idx = std::stoi(token);

                // Convertir índices negativos
                if (idx < 0)
                    idx = static_cast<int>(positions.size()) + idx + 1;

                faceIndices.push_back(idx - 1); // base 1
            }

            // Asignar un color según el número de cara
            const float *color = colors[data.faceCount % numColors];

            // Triangular la cara
            for (size_t i = 1; i + 1 < faceIndices.size(); i++)
            {
                int i0 = faceIndices[0];
                int i1 = faceIndices[i];
                int i2 = faceIndices[i + 1];

                // Cada vértice: posición (3 floats) + color (3 floats)
                data.vertices.push_back(positions[i0].x);
                data.vertices.push_back(positions[i0].y);
                data.vertices.push_back(positions[i0].z);
                data.vertices.push_back(color[0]);
                data.vertices.push_back(color[1]);
                data.vertices.push_back(color[2]);

                data.vertices.push_back(positions[i1].x);
                data.vertices.push_back(positions[i1].y);
                data.vertices.push_back(positions[i1].z);
                data.vertices.push_back(color[0]);
                data.vertices.push_back(color[1]);
                data.vertices.push_back(color[2]);

                data.vertices.push_back(positions[i2].x);
                data.vertices.push_back(positions[i2].y);
                data.vertices.push_back(positions[i2].z);
                data.vertices.push_back(color[0]);
                data.vertices.push_back(color[1]);
                data.vertices.push_back(color[2]);
            }

            data.faceCount++;
        }
    }

    data.vertexCount = static_cast<int>(positions.size());

    // Calcular bounding box recorriendo todos los vértices
    if (!positions.empty())
    {
        data.minBound = positions[0];
        data.maxBound = positions[0];
        for (const auto &p : positions)
        {
            if (p.x < data.minBound.x)
                data.minBound.x = p.x;
            if (p.y < data.minBound.y)
                data.minBound.y = p.y;
            if (p.z < data.minBound.z)
                data.minBound.z = p.z;
            if (p.x > data.maxBound.x)
                data.maxBound.x = p.x;
            if (p.y > data.maxBound.y)
                data.maxBound.y = p.y;
            if (p.z > data.maxBound.z)
                data.maxBound.z = p.z;
        }
    }

    // Imprimir información del modelo en la terminal
    const char *teal = "\033[38;2;0;128;128m";
    const char *reset = "\033[0m";

    float sizeX = data.maxBound.x - data.minBound.x;
    float sizeY = data.maxBound.y - data.minBound.y;
    float sizeZ = data.maxBound.z - data.minBound.z;
    float cx = (data.minBound.x + data.maxBound.x) / 2.0f;
    float cy = (data.minBound.y + data.maxBound.y) / 2.0f;
    float cz = (data.minBound.z + data.maxBound.z) / 2.0f;
    int triangles = static_cast<int>(data.vertices.size()) / 18;

    // std::cout << "\n";
    std::cout << teal << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << reset << "\n";
    std::cout << teal << "\u2551" << reset << "        Modelo OBJ Cargado        " << teal << "\u2551" << reset << "\n";
    std::cout << teal << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << reset << "\n";
    std::cout << "  Archivo:    " << path << "\n";
    std::cout << "  V\u00e9rtices:   " << data.vertexCount << "\n";
    std::cout << "  Caras:      " << data.faceCount    << "\n";
    std::cout << "  Tri\u00e1ngulos: " << triangles        << "\n";
    std::cout << std::fixed << std::setprecision(3);
    float radius = 0.0f;
    if (!positions.empty())
    {
        float dx = data.maxBound.x - data.minBound.x;
        float dy = data.maxBound.y - data.minBound.y;
        float dz = data.maxBound.z - data.minBound.z;
        radius = std::sqrt(dx*dx + dy*dy + dz*dz) * 0.5f;
    }
    std::cout << "  Tama\u00f1o:     " << sizeX << " x " << sizeY << " x " << sizeZ << "\n";
    std::cout << "  Radio:      " << radius << "\n";
    std::cout << "  Centro:     " << cx << ", " << cy << ", " << cz << "\n";
    std::cout << "\n";

    return data;
}
