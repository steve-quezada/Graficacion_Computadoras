// Modelo cargado desde un archivo .obj.

#pragma once
#include "models/Model.h"
#include "math/Vector3D.h"
#include <vector>
#include <string>

class CustomModel : public Model
{
public:
    // Carga el obj, sube los datos a la GPU y calcula el bounding box
    CustomModel(ShaderProgram *program, const std::string &path);

    // Dibuja el modelo con glDrawArrays
    void render(const Matrix4D &view, const Matrix4D &projection) override;

    // Centro del bounding box
    Vector3D getCenter() const;

    // Radio del bounding box
    float getBoundingRadius() const;

    // Nombre del archivo cargado
    std::string getName() const;

private:
    int m_drawCount;     // número de vértices a dibujar
    Vector3D m_minBound; // esquina mínima del bounding box
    Vector3D m_maxBound; // esquina máxima del bounding box
    std::string m_name;  // nombre del archivo

    // Sube los vértices a la GPU
    void initBuffers(const std::vector<float> &vertices);
};
