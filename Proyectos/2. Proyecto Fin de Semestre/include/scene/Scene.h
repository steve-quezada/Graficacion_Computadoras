// Crea la ventana, el shader, los modelos y la cámara.
// Procesa la entrada del teclado y ejecuta el ciclo de render.

#pragma once
#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/Camera.h"
#include "models/CustomModel.h"
#include "models/Axes.h"
#include "models/Room.h"
#include "math/Vector3D.h"
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
    // Recursos
    Window *m_window;             // Ventana y contexto OpenGL
    ShaderProgram *m_shader;      // Shader Modelo de Phong y Texturas
    ShaderProgram *m_axesShader;  // Shader ejes XYZ
    Camera *m_camera;             // Cámara FPS
    Axes *m_axes;                 // Ejes XYZ
    Room *m_room;                 // Habitación explorable

    std::vector<CustomModel *> m_models; // Modelos cargados desde OBJ

    int m_renderMode;                    // 0 = fill 
                                         // 1 = wire 
                                         // 2 = puntos

    // Iluminación
    Vector3D m_lightPos;
    Vector3D m_lightColor;

    // Mouse FPS
    double m_lastMouseX{0}, m_lastMouseY{0};
    bool m_firstMouse{true};

    float m_birdAngle{0.0f}; // Ángulo de rotación del modelo animado

    // Teclas para cambiar modos
    bool m_tabWasPressed{false};
    bool m_fWasPressed{false};
    bool m_cWasPressed{false};
    bool m_rWasPressed{false};

    bool m_requireClick{true};
    bool m_mWasPressed{false};

    // Inicialización
    void init();
    void loadModels();
    void positionModels(); // normaliza y ubica los modelos en la escena

    // Bucle
    void processInput(float deltaTime);

    // Uniforms de luz/cámara
    void setLightUniforms();

    // Callback del ratón
    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    void onMouseMove(double xpos, double ypos);
};
