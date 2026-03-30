// Implementación del cargador de archivos .obj.

#include "loaders/ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

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

    // Paleta de colores (ampliada)
    const float colors[][3] = {
        {0.90f, 0.30f, 0.30f}, // rojo coral
        {0.30f, 0.80f, 0.30f}, // verde brillante
        {0.30f, 0.50f, 0.90f}, // azul medio
        {0.90f, 0.80f, 0.20f}, // amarillo dorado
        {0.90f, 0.50f, 0.20f}, // naranja fuerte
        {0.60f, 0.30f, 0.80f}, // violeta
        {0.20f, 0.80f, 0.80f}, // turquesa
        {0.90f, 0.40f, 0.70f}, // rosa intenso
        {0.50f, 0.80f, 0.50f}, // verde claro
        {0.80f, 0.60f, 0.40f}, // marrón claro
        {0.40f, 0.70f, 0.90f}, // azul cielo
        {0.90f, 0.70f, 0.70f}, // rosa salmón
        {0.50f, 0.50f, 0.90f}, // lavanda
        {0.70f, 0.90f, 0.30f}, // verde lima
        {0.90f, 0.30f, 0.60f}, // fucsia
        {0.30f, 0.60f, 0.60f}, // teal
        {0.45f, 0.30f, 0.75f}, // índigo
        {0.20f, 0.50f, 0.20f}, // verde oscuro
        {0.95f, 0.85f, 0.15f}, // dorado brillante
        {0.45f, 0.25f, 0.15f}, // marrón oscuro
        {0.25f, 0.50f, 0.75f}, // azul acero
        {0.75f, 0.25f, 0.25f}, // rojo ladrillo
        {0.65f, 0.85f, 0.90f}, // cian claro
        {0.85f, 0.65f, 0.95f}, // lila claro
        {0.35f, 0.20f, 0.60f}, // púrpura
        {0.15f, 0.60f, 0.40f}, // verde mar
        {0.95f, 0.45f, 0.15f}, // naranja
        {0.60f, 0.60f, 0.60f}, // gris medio
        {0.25f, 0.70f, 0.95f}, // azul cielo
        {0.98f, 0.75f, 0.30f}, // melocotón
        {0.40f, 0.90f, 0.60f}, // verde menta
        {0.20f, 0.40f, 0.90f}, // azul real
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

    std::cout << "\n";
    std::cout << teal << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << reset << "\n";
    std::cout << teal << "\u2551" << reset << "        Modelo OBJ Cargado        " << teal << "\u2551" << reset << "\n";
    std::cout << teal << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << reset << "\n";
    std::cout << "  Archivo:    " << path << "\n";
    std::cout << "  V\u00e9rtices:   " << data.vertexCount << "\n";
    std::cout << "  Caras:      " << data.faceCount << "\n";
    std::cout << "  Tri\u00e1ngulos: " << triangles << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Tama\u00f1o:     " << sizeX << " x " << sizeY << " x " << sizeZ << "\n";
    std::cout << "  Centro:     " << cx << ", " << cy << ", " << cz << "\n";
    std::cout << "\n";

    return data;
}
