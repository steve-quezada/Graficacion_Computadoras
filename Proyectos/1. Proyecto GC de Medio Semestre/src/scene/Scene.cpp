// Implementación de la escena.

#include "scene/Scene.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <filesystem>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const char *TEAL = "\033[38;2;0;128;128m";
static const char *RESET = "\033[0m";

Scene::Scene()
{
    init();
}

Scene::~Scene()
{
    for (auto *m : m_models)
        delete m;
    delete m_axes;
    delete m_shader;
    delete m_camera;
    delete m_window;
}

void Scene::init()
{
    m_window = new Window(800, 600, "Proyecto - Medio Semestre");

    m_shader = new ShaderProgram("shaders/vert/vertex.vert", "shaders/frag/fragment.frag");

    // Cámara orbital
    float fov = 45.0f * static_cast<float>(M_PI) / 180.0f;
    m_camera = new Camera(
        Vector3D{0.0f, 0.0f, 0.0f}, // target: centro de la escena
        5.0f,                       // radius
        0.5f,                       // pitch
        0.0f,                       // yaw
        fov,                        // fov (radianes)
        0.001f,                     // near
        1000000.0f                  // far
    );

    m_axes = new Axes(m_shader);

    m_currentModel = 0;
    m_renderMode = 0;
    m_tabWasPressed = false;
    m_fWasPressed = false;

    std::cout << "\n";
    std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
    std::cout << TEAL << "\u2551" << RESET << "       Visor de Modelos OBJ       " << TEAL << "\u2551" << RESET << "\n";
    std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
    std::cout << "\n";

    std::cout << TEAL << "[FASE 1]" << RESET << " Cargando modelos\n\n";
    loadModels();

    if (!m_models.empty())
    {
        std::cout << TEAL << "[FASE 2]" << RESET << " Iniciando visor\n\n";
        focusCurrentModel();
    }
}

void Scene::loadModels()
{
    namespace fs = std::filesystem;
    const std::string objDir = "obj/";

    if (!fs::exists(objDir) || !fs::is_directory(objDir))
    {
        std::cerr << "ERROR: carpeta '" << objDir << "' no encontrada." << std::endl;
        return;
    }

    // Recolectar y ordenar los archivos .obj alfabeticamente
    std::vector<std::string> paths;
    for (const auto &entry : fs::directory_iterator(objDir))
    {
        if (entry.path().extension() == ".obj")
            paths.push_back(entry.path().string());
    }
    std::sort(paths.begin(), paths.end());

    for (const auto &path : paths)
        m_models.push_back(new CustomModel(m_shader, path));
}

void Scene::focusCurrentModel()
{
    CustomModel *model = m_models[m_currentModel];
    Vector3D center = model->getCenter();

    float radius = model->getBoundingRadius();
    m_camera->resetView(center, radius * 3.5f);

    float axisScale = radius * 1.0f;
    m_axes->setModelMatrix(
        Matrix4D::translate(center) *
        Matrix4D::scale({axisScale, axisScale, axisScale}));

    std::cout << TEAL << "  \u2192 Modelo activo: " << RESET << model->getName() << "\n\n";
}

void Scene::processInput(float deltaTime)
{
    // ESC: cerrar ventana
    if (glfwGetKey(m_window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->handle, true);

    // Tab: siguiente modelo
    bool tabNow = glfwGetKey(m_window->handle, GLFW_KEY_TAB) == GLFW_PRESS;
    if (tabNow && !m_tabWasPressed && !m_models.empty())
    {
        m_currentModel = (m_currentModel + 1) % static_cast<int>(m_models.size());
        m_models[m_currentModel]->resetTransform();
        focusCurrentModel();
    }
    m_tabWasPressed = tabNow;

    // F: resetear cámara
    bool fNow = glfwGetKey(m_window->handle, GLFW_KEY_F) == GLFW_PRESS;
    if (fNow && !m_fWasPressed)
        focusCurrentModel();
    m_fWasPressed = fNow;

    // 1: modo superfice
    if (glfwGetKey(m_window->handle, GLFW_KEY_1) == GLFW_PRESS && m_renderMode != 0)
    {
        m_renderMode = 0;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "         Modo: Superficie         " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
        std::cout << "\n";
    }
    // 2: modo wireframe
    if (glfwGetKey(m_window->handle, GLFW_KEY_2) == GLFW_PRESS && m_renderMode != 1)
    {
        m_renderMode = 1;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "         Modo: Wireframe          " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
        std::cout << "\n";
    }
    // 3: modo nube de puntos
    if (glfwGetKey(m_window->handle, GLFW_KEY_3) == GLFW_PRESS && m_renderMode != 2)
    {
        m_renderMode = 2;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "       Modo: Nube de Puntos       " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
        std::cout << "\n";
    }

    // Detectar modificadores
    bool shift = glfwGetKey(m_window->handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                 glfwGetKey(m_window->handle, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    bool ctrl = glfwGetKey(m_window->handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                glfwGetKey(m_window->handle, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

    // Velocidad base proporcional al tamaño del modelo
    float baseSpeed = 1.0f;
    if (!m_models.empty())
        baseSpeed = m_models[m_currentModel]->getBoundingRadius();

    // Camara
    if (!shift && !ctrl)
    {
        // WASD: orbitar
        float orbitSpeed = 2.0f;
        float deltaYaw = 0.0f;
        float deltaPitch = 0.0f;

        if (glfwGetKey(m_window->handle, GLFW_KEY_A) == GLFW_PRESS)
            deltaYaw -= orbitSpeed * deltaTime;
        if (glfwGetKey(m_window->handle, GLFW_KEY_D) == GLFW_PRESS)
            deltaYaw += orbitSpeed * deltaTime;
        if (glfwGetKey(m_window->handle, GLFW_KEY_W) == GLFW_PRESS)
            deltaPitch += orbitSpeed * deltaTime;
        if (glfwGetKey(m_window->handle, GLFW_KEY_S) == GLFW_PRESS)
            deltaPitch -= orbitSpeed * deltaTime;

        if (deltaYaw != 0.0f || deltaPitch != 0.0f)
            m_camera->orbit(deltaYaw, deltaPitch);

        // Q/E: zoom
        float zoomSpeed = baseSpeed * 2.0f;
        if (glfwGetKey(m_window->handle, GLFW_KEY_Q) == GLFW_PRESS)
            m_camera->zoom(-zoomSpeed * deltaTime);
        if (glfwGetKey(m_window->handle, GLFW_KEY_E) == GLFW_PRESS)
            m_camera->zoom(zoomSpeed * deltaTime);
    }

    if (m_models.empty())
        return;

    float rotSpeed = 90.0f;
    float moveSpeed = baseSpeed;

    // Modelo
    
    // Shift + WASD/QE: rotar
    if (shift && !ctrl)
    {
        if (glfwGetKey(m_window->handle, GLFW_KEY_A) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(rotSpeed * deltaTime, 'Y');
        if (glfwGetKey(m_window->handle, GLFW_KEY_D) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(-rotSpeed * deltaTime, 'Y');
        if (glfwGetKey(m_window->handle, GLFW_KEY_W) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(rotSpeed * deltaTime, 'X');
        if (glfwGetKey(m_window->handle, GLFW_KEY_S) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(-rotSpeed * deltaTime, 'X');
        if (glfwGetKey(m_window->handle, GLFW_KEY_Q) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(rotSpeed * deltaTime, 'Z');
        if (glfwGetKey(m_window->handle, GLFW_KEY_E) == GLFW_PRESS)
            m_models[m_currentModel]->rotate(-rotSpeed * deltaTime, 'Z');
    }
    //  Ctrl + WASD/QE: trasladar
    if (ctrl && !shift)
    {
        if (glfwGetKey(m_window->handle, GLFW_KEY_A) == GLFW_PRESS)
            m_models[m_currentModel]->translate(-moveSpeed * deltaTime, 0, 0);
        if (glfwGetKey(m_window->handle, GLFW_KEY_D) == GLFW_PRESS)
            m_models[m_currentModel]->translate(moveSpeed * deltaTime, 0, 0);
        if (glfwGetKey(m_window->handle, GLFW_KEY_W) == GLFW_PRESS)
            m_models[m_currentModel]->translate(0, moveSpeed * deltaTime, 0);
        if (glfwGetKey(m_window->handle, GLFW_KEY_S) == GLFW_PRESS)
            m_models[m_currentModel]->translate(0, -moveSpeed * deltaTime, 0);
        if (glfwGetKey(m_window->handle, GLFW_KEY_Q) == GLFW_PRESS)
            m_models[m_currentModel]->translate(0, 0, -moveSpeed * deltaTime);
        if (glfwGetKey(m_window->handle, GLFW_KEY_E) == GLFW_PRESS)
            m_models[m_currentModel]->translate(0, 0, moveSpeed * deltaTime);
    }

    // R: resetear transformación del modelo
    if (glfwGetKey(m_window->handle, GLFW_KEY_R) == GLFW_PRESS)
        m_models[m_currentModel]->resetTransform();
}

void Scene::render()
{
    glEnable(GL_DEPTH_TEST);

    double lastFrame = glfwGetTime();

    while (!m_window->shouldClose())
    {
        // Delta Time: tiempo entre frames
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;

        processInput(deltaTime);

        // glClearColor(0.03f, 0.12f, 0.12f, 1.0f);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix4D view = m_camera->getViewMatrix();
        Matrix4D projection = m_camera->getProjectionMatrix(m_window->getAspectRatio());

        // Dibujar ejes siempre como líneas
        m_axes->render(view, projection);

        // Aplicar modo de renderizado al modelo actual
        switch (m_renderMode)
        {
        case 0:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 2:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glPointSize(2.0f);
            break;
        }

        if (!m_models.empty())
            m_models[m_currentModel]->render(view, projection);

        // Restaurar modo de relleno para los ejes del siguiente frame
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_window->swapBuffers();
        m_window->pollEvents();
    }

    std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
    std::cout << TEAL << "\u2551" << RESET << "         Sesion terminada         " << TEAL << "\u2551" << RESET << "\n";
    std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
    std::cout << "\n";
}
