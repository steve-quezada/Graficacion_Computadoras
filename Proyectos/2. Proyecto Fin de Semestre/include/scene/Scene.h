// Crea la ventana, el shader, los modelos y la cámara.
// Procesa la entrada del teclado y ejecuta el ciclo de render.

#pragma once
#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/Camera.h"
#include "models/CustomModel.h"
#include "models/Axes.h"
#include "models/Room.h"
#include "models/SkyBox.h"
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
    Window *m_window{nullptr};              // Ventana y contexto OpenGL
    ShaderProgram *m_shader{nullptr};       // Shader Modelo de Phong y Texturas
    ShaderProgram *m_axesShader{nullptr};   // Shader ejes XYZ
    ShaderProgram *m_normalShader{nullptr}; // Shader para renderizar normales
    ShaderProgram *m_skyboxShader{nullptr}; // Shader para skybox

    // Cámara
    Camera *m_camera{nullptr}; // Cámara orbital con opción a modo libre

    // Modelos
    Axes *m_axes{nullptr};               // Ejes XYZ
    Room *m_room{nullptr};               // Habitación explorable
    SkyBox *m_skybox{nullptr};           // Skybox con textura cubemap
    std::vector<CustomModel *> m_models; // Modelos cargados desde OBJ

    int m_renderMode{0}; // 0 fill | 1 wire | 2 points

    // 3 luces: Luna | Baliza | UFO Spotlight
    static const int NUM_LIGHTS = 3;
    Vector3D m_lightPos[NUM_LIGHTS];
    Vector3D m_lightAmbient[NUM_LIGHTS];
    Vector3D m_lightDiffuse[NUM_LIGHTS];
    Vector3D m_lightSpecular[NUM_LIGHTS];

    // Toggles
    bool m_lightsOn{true};  // L: encender/apagar luces
    bool m_shadersOn{true}; // F: encender/apagar shaders Phong
    bool m_axesOn{false};   // X: mostrar/ocultar ejes

    // Animaciones
    float m_birdAngle{0.0f};

    // UFO: ciclo  Espera → Desciende → Queda → Sube → Espera →
    enum class UFOPhase
    {
        Waiting,
        Descending,
        Staying,
        Ascending
    };

    // UFO
    CustomModel *m_ufoModel{nullptr};
    UFOPhase m_ufoPhase{UFOPhase::Waiting};
    float m_ufoTimer{0.0f};     // tiempo acumulado
    float m_ufoY{200.0f};       // posición Y actual
    float m_ufoStartY{200.0f};  // altura inicial
    float m_ufoLandY{6.0f};     // altura de aterrizaje
    float m_ufoSpeed{20.0f};    // m/s de descenso y ascenso
    float m_ufoWaitTime{30.0f}; // segundos de espera antes de aparecer
    float m_ufoStayTime{15.0f}; // segundos posado en tierra
    bool m_ufoVisible{false};   // false = no se renderiza

    // Edge-detection de teclas
    bool m_cWasPressed{false};
    bool m_lWasPressed{false};
    bool m_fWasPressed{false};
    bool m_xWasPressed{false};
    bool m_pWasPressed{false};

    // Pausa del UFO
    bool m_ufoPaused{false}; // P: pausar/reanudar animación del UFO

    // Inicialización
    void init();
    void loadModels();
    void positionModels();

    // Bucle
    void processInput(float deltaTime);

    // Envía uniforms de iluminación al shader indicado
    void setLightUniforms(ShaderProgram *sh);
};
