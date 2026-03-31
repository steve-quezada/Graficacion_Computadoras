// Crea la ventana, el shader, los modelos y la cámara.
// Procesa la entrada del teclado y ejecuta el ciclo de render.

#pragma once
#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/Camera.h"
#include "models/CustomModel.h"
#include "models/Axes.h"
#include <vector>
#include <string>

class Scene
{
public:
    // Llama a init() para crear todos los recursos
    Scene();

    // Libera todos los recursos
    ~Scene();

    // Procesa entrada, actualiza y renderiza hasta cerrar
    void render();

private:
    Window *m_window;
    ShaderProgram *m_shader;
    Camera *m_camera;
    Axes *m_axes;

    std::vector<CustomModel *> m_models; // modelos obj cargados
    int m_currentModel;                  // índice del modelo activo
    int m_renderMode;                    // 0 = superfice, 1 = wireframe, 2 = puntos
    bool m_tabWasPressed;                // edge detection para Tab
    bool m_fWasPressed;                  // edge detection para F

    // Crea ventana, shader, cámara, ejes y carga los modelos obj
    void init();

    // Carga todos los archivos .obj disponibles
    void loadModels();

    // Centra la cámara en el modelo actual usando su bounding box
    void focusCurrentModel();

    // Lee el teclado: orbitar cámara, cambiar modelo, cambiar modo de render
    void processInput(float deltaTime);
};
