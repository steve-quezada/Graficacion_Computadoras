// Habitación explorable

#pragma once
#include "models/Model.h"
#include "math/Matrix3D.h"

class Room : public Model
{
public:
    // Crea un cubo centrado en el origen.
    // Las 6 caras tienen normales apuntando hacia adentro.

    // Carga tres texturas desde assets/textures/room
    Room(ShaderProgram *program, float halfSize = 10.0f);

    ~Room() override;

    // Dibuja las caras interiores
    void render(const Matrix4D &view, const Matrix4D &projection) override;

    float getHalfSize() const { return m_halfSize; }

private:
    int m_indexCount;  // Número total de índices
    GLuint m_texFloor; // Textura del piso
    GLuint m_texCeil;  // Textura del techo
    GLuint m_texWall;  // Textura de las paredes
    float m_halfSize;  // Distancia del centro a cada pared

    // Inicializa los buffers con la geometría
    void initGeometry(float h);
};
