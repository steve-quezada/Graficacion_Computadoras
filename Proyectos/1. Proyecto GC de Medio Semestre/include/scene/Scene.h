// Crea la ventana, el shader, el cubo y la cámara.
// Procesa la entrada del teclado y ejecuta el ciclo de render.

#pragma once
#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/Camera.h"
#include "models/Model.h"
#include "models/Cube.h"

class Scene
{
public:
    // Llama a init() para crear todos los recursos
    Scene();

    // Libera todos los recursos (modelos, shader, cámara, ventana)
    ~Scene();

    // Procesa entrada, actualiza y renderiza hasta cerrar
    void render();

private:
    Window *m_window;
    ShaderProgram *m_shader;
    Model *m_cube;
    Camera *m_camera;

    bool m_rotating;        // true = el cubo rota, false = el cubo no rota
    bool m_spaceWasPressed; // Guarda el estado del Espacio

    // Crea ventana, shader, cubo y cámara con sus parámetros iniciales
    void init();

    // Lee el teclado y orbita la cámara o pausa/reanuda la rotación
    void processInput(float deltaTime);
};
