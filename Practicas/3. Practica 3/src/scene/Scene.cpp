// Implementación de la escena.

#include "scene/Scene.h"
#include <cmath>

// M_PI no siempre está definido
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Scene::Scene()
{
    init();
}

Scene::~Scene()
{
    delete m_cube;
    delete m_shader;
    delete m_camera;
    delete m_window;
}

void Scene::init()
{
    m_window = new Window(800, 600, "Practica 3 - Cubo 3D");

    m_shader = new ShaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    m_cube = new Cube(m_shader);

    // Cámara orbital: 3 unidades de distancia, ~28° de elevación, fov 45°
    float fov = 45.0f * static_cast<float>(M_PI) / 180.0f;
    m_camera = new Camera(
        Vector3D{0.0f, 0.0f, 0.0f}, // target: centro de la escena
        3.0f,                       // radius
        0.5f,                       // pitch (~28°)
        0.0f,                       // yaw
        fov,                        // fov (radianes)
        0.1f,                       // near
        100.0f                      // far
    );

    m_rotating = true;
    m_spaceWasPressed = false;
}

void Scene::processInput(float deltaTime)
{
    // ESC para cerrar
    if (glfwGetKey(m_window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->handle, true);

    // Espacio: pausar/reanudar la rotación del cubo
    bool spaceNow = glfwGetKey(m_window->handle, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spaceNow && !m_spaceWasPressed)
        m_rotating = !m_rotating;
    m_spaceWasPressed = spaceNow;

    // A/D o Flechas izquierda/derecha: orbitar la cámara horizontalmente
    float orbitSpeed = 2.0f;
    float deltaYaw = 0.0f;
    float deltaPitch = 0.0f;

    if (glfwGetKey(m_window->handle, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(m_window->handle, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        deltaYaw -= orbitSpeed * deltaTime;
    }
    if (glfwGetKey(m_window->handle, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(m_window->handle, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        deltaYaw += orbitSpeed * deltaTime;
    }

    // W/S o Flechas arriba/abajo: orbitar verticalmente
    if (glfwGetKey(m_window->handle, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(m_window->handle, GLFW_KEY_UP) == GLFW_PRESS)
    {
        deltaPitch += orbitSpeed * deltaTime;
    }
    if (glfwGetKey(m_window->handle, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(m_window->handle, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        deltaPitch -= orbitSpeed * deltaTime;
    }

    // Solo orbitar si hubo movimiento
    if (deltaYaw != 0.0f || deltaPitch != 0.0f)
    {
        m_camera->orbit(deltaYaw, deltaPitch);
    }
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

        glClearColor(0.03f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Obtener matrices de cámara
        Matrix4D view = m_camera->getViewMatrix();
        Matrix4D projection = m_camera->getProjectionMatrix(m_window->getAspectRatio());

        if (m_rotating)
            m_cube->update(deltaTime);
        m_cube->render(view, projection);

        m_window->swapBuffers();
        m_window->pollEvents();
    }
}
