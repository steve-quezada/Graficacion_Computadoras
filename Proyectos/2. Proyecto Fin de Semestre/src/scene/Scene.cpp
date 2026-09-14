// Escena principal - Proyecto Fin de Semestre
// 4 modelos OBJ + 3 luces + texturas + skybox + normal mapping
// Controles: WASD/flechas (cámara), C (orbital/libre), 1/2/3 (render), L (luces), F (shaders)

#include "scene/Scene.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <filesystem>
#include <algorithm>

static const float PI = 3.14159265f;
static const float CAMERA_MIN_Y = -4.0f;

static const char *TEAL = "\033[38;2;0;128;128m";
static const char *RESET = "\033[0m";

Scene::Scene()
{
    init();
}

Scene::~Scene()
{
    for (auto *m : m_models) delete m;
    delete m_skybox;
    delete m_room;
    delete m_axes;
    delete m_skyboxShader;
    delete m_normalShader;
    delete m_axesShader;
    delete m_shader;
    delete m_camera;
    delete m_window;
}

void Scene::init()
{
    m_window = new Window(1280, 720, "Proyecto Fin de Semestre - Espacio Escultórico");
    m_shader       = new ShaderProgram("shaders/vert/vertex.vert",  "shaders/frag/fragment.frag");
    m_axesShader   = new ShaderProgram("shaders/vert/axes.vert",    "shaders/frag/axes.frag");
    m_normalShader = new ShaderProgram("shaders/vert/normal.vert",  "shaders/frag/normal.frag");
    m_skyboxShader = new ShaderProgram("shaders/vert/skybox.vert",  "shaders/frag/skybox.frag");

    // Cámara en x+,y+,z+ mirando al centro de la escena
    float fov = 60.0f * PI / 180.0f;
    m_camera = new Camera(
        Vector3D{0.0f, 0.0f, 0.0f},  // target: centro de la escena
        150.0f,                      // radius: distancia inicial al target
        0.25f,                       // pitch: ángulo vertical
        PI * 1.25f,                  // yaw: ángulo horizontal
        fov,                         // fov (radianes)
        0.5f,                        // near
        2000.0f);                    // far

    // Ejes
    m_axes = new Axes(m_axesShader);
    m_axes->setModelMatrix(
        Matrix4D::translate({0.f, 1.5f, 0.f}) *
        Matrix4D::scale({50.f, 50.f, 50.f}));

    // Luz 0: Luz lunar (azul-gris)
    m_lightPos[0]      = Vector3D{  200.f,  400.f,  100.f}; // posición alta y lateral
    m_lightAmbient[0]  = Vector3D{  0.06f,  0.07f,  0.10f}; // ambiente nocturno
    m_lightDiffuse[0]  = Vector3D{  0.42f,  0.48f,  0.58f}; // difuso frío
    m_lightSpecular[0] = Vector3D{  0.16f,  0.18f,  0.26f}; // especular tenue

    // Luz 1: Baliza (roja)
    m_lightPos[1]      = Vector3D{  0.f,   -1.0f,   0.f};    // ligeramente bajo tierra
    m_lightAmbient[1]  = Vector3D{  0.015f, 0.002f, 0.002f}; // ambiente tenue
    m_lightDiffuse[1]  = Vector3D{  0.45f,  0.06f,  0.05f};  // rojo guía
    m_lightSpecular[1] = Vector3D{  0.20f,  0.03f,  0.03f};  // rojo especular

    // Luz 2: UFO Spotlight (verde) [Se actualiza cada frame]
    m_lightPos[2]      = Vector3D{  0.f,  m_ufoStartY, 0.f}; // posición inicial
    m_lightAmbient[2]  = Vector3D{  0.0f,  0.0f,  0.0f};     // cero cuando no hay nave
    m_lightDiffuse[2]  = Vector3D{  0.0f,  0.0f,  0.0f};     // se actualiza a verde brillante
    m_lightSpecular[2] = Vector3D{  0.0f,  0.0f,  0.0f};     // se actualiza a verde brillante

    m_renderMode = 0;

    std::cout << "\n";
    std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
    std::cout << TEAL << "\u2551" << RESET << "  Proyecto Fin de Semestre - Espacio Escultórico  " << TEAL << "\u2551" << RESET << "\n";
    std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
    std::cout << "\n";
    std::cout << TEAL << "[FASE 1]" << RESET << " Cargando modelos\n\n";

    loadModels();
    positionModels();

    std::cout << TEAL << "[FASE 2]" << RESET << " Iniciando ciclo de render\n\n";
    std::cout << "  Controles:\n";
    std::cout << "    W / S         Avanzar / Retroceder\n";
    std::cout << "    A / D         Mover Izquierda / Derecha\n";
    std::cout << "    Q / E         Subir / Bajar (libre) | Zoom (orbital)\n";
    std::cout << "    Flechas       Rotar / Orbitar Vista\n";
    std::cout << "    C             Alternar Camara (Libre / Orbital)\n";
    std::cout << "    L             Encender / Apagar Luces\n";
    std::cout << "    F             Encender / Apagar Shaders (Phong)\n";
    std::cout << "    X             Mostrar / Ocultar Ejes\n";
    std::cout << "    1 / 2 / 3     Modo Render (fill / wireframe / puntos)\n";
    std::cout << "    ESC           Salir\n\n";
}

void Scene::loadModels()
{
    namespace fs = std::filesystem;
    const std::string objDir = "assets/obj/";

    if (!fs::exists(objDir) || !fs::is_directory(objDir))
    {
        std::cerr << "ERROR: carpeta '" << objDir << "' no encontrada.\n";
        return;
    }

    // Skybox
    m_skybox = new SkyBox(m_skyboxShader, "assets/skybox/");
    std::cout << "  Skybox cargado\n\n";

    std::vector<std::string> paths;
    for (const auto &entry : fs::directory_iterator(objDir))
    {
        if (entry.path().extension() == ".obj")
            paths.push_back(entry.path().string());
    }
    std::sort(paths.begin(), paths.end());

    for (const auto &path : paths)
    {
        std::string stem     = fs::path(path).stem().string();
        std::string texPath  = "assets/textures/" + stem + ".jpg";
        auto *mdl = new CustomModel(m_shader, path.c_str(), texPath.c_str());
        m_models.push_back(mdl);

        // UFO: cargar normal map metálico
        std::string lower = stem;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (lower.find("ufo") != std::string::npos)
        {
            m_ufoModel = mdl;
            mdl->loadNormalMap("assets/textures/UFO_normal.jpg");
        }

        std::cout << "  [OBJ] " << stem << "\n\n";
    }
}

void Scene::positionModels()
{
    if (m_models.empty())
        return;

    for (CustomModel *m : m_models)
    {
        const std::string &name = m->getName();
        Vector3D center = m->getCenter();
        float radius = m->getBoundingRadius();
        if (radius < 1e-6f)
            radius = 1.0f;

        // Nombre en minúsculas para comparaciones
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        Matrix4D mat;

        if (lower.find("ufo") != std::string::npos)
        {
            // UFO: posición inicial alta
            float s = 25.0f / radius;
            mat = Matrix4D::translate({0.0f, m_ufoStartY, 0.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (lower.find("beacon") != std::string::npos)
        {
            // Baliza: levemente bajo el nivel del suelo
            float s = 1.5f / radius;
            mat = Matrix4D::translate({0.0f, -2.5f, 0.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (lower.find("beagle") != std::string::npos)
        {
            float s = 4.0f / radius;
            mat = Matrix4D::translate({55.0f, 2.0f, -40.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::rotateX(-PI * 0.5f)
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (lower.find("cat") != std::string::npos ||
                 lower.find("banana") != std::string::npos)
        {
            // Banana-Cat: se sobreescribe en render loop
            float s = 2.5f / radius;
            mat = Matrix4D::translate({0.0f, 1.0f, -75.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::rotateX(-PI * 0.5f)
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (lower.find("spaceship") != std::string::npos ||
                 lower.find("duck") != std::string::npos)
        {
            // Nave pequeña: posición inicial
            float s = 6.0f / radius;
            mat = Matrix4D::translate({100.0f, 40.0f, 0.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::rotateX(-PI * 0.5f)
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (lower.find("toad") != std::string::npos)
        {
            float s = 2.5f / radius;
            mat = Matrix4D::translate({70.0f, 1.0f, -65.0f})
                * Matrix4D::scale({s, s, s})
                * Matrix4D::rotateX(-PI * 0.5f)
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else
        {
            // Espacio Escultórico: centrado en origen
            float s = 80.0f / radius;
            mat = Matrix4D::scale({s, s, s})
                * Matrix4D::translate({-center.x, -center.y, -center.z});
        }

        m->setModelMatrix(mat);
    }
}

void Scene::processInput(float deltaTime)
{
    GLFWwindow *win = m_window->handle;

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    // C: cámara orbital / libre
    bool cNow = glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS;
    if (cNow && !m_cWasPressed)
    {
        m_camera->toggleFreeMode();
        if (m_camera->isFreeMode())
        {
            std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
            std::cout << TEAL << "\u2551" << RESET << "         Camara: Libre            " << TEAL << "\u2551" << RESET << "\n";
            std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n\n";
        }
        else
        {
            std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
            std::cout << TEAL << "\u2551" << RESET << "         Camara: Orbital          " << TEAL << "\u2551" << RESET << "\n";
            std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n\n";
        }
    }
    m_cWasPressed = cNow;

    // L: encender / apagar luces
    bool lNow = glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS;
    if (lNow && !m_lWasPressed)
    {
        m_lightsOn = !m_lightsOn;
        std::cout << TEAL << "╔══════════════════════════════════╗" << RESET << "\n";
        if (m_lightsOn)
            std::cout << TEAL << "║" << RESET << "         Luces: Encendidas        " << TEAL << "║" << RESET << "\n";
        else
            std::cout << TEAL << "║" << RESET << "          Luces: Apagadas         " << TEAL << "║" << RESET << "\n";
        std::cout << TEAL << "╚══════════════════════════════════╝" << RESET << "\n\n";
    }
    m_lWasPressed = lNow;

    // F: encender / apagar shaders
    bool fNow = glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS;
    if (fNow && !m_fWasPressed)
    {
        m_shadersOn = !m_shadersOn;
        std::cout << TEAL << "╔══════════════════════════════════╗" << RESET << "\n";
        if (m_shadersOn)
            std::cout << TEAL << "║" << RESET << "        Shaders: Encendidos       " << TEAL << "║" << RESET << "\n";
        else
            std::cout << TEAL << "║" << RESET << "         Shaders: Apagados        " << TEAL << "║" << RESET << "\n";
        std::cout << TEAL << "╚══════════════════════════════════╝" << RESET << "\n\n";
    }
    m_fWasPressed = fNow;

    // X: mostrar / ocultar ejes
    bool xNow = glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS;
    if (xNow && !m_xWasPressed)
    {
        m_axesOn = !m_axesOn;
        std::cout << TEAL << "╔══════════════════════════════════╗" << RESET << "\n";
        if (m_axesOn)
            std::cout << TEAL << "║" << RESET << "          Ejes: Visibles          " << TEAL << "║" << RESET << "\n";
        else
            std::cout << TEAL << "║" << RESET << "           Ejes: Ocultos          " << TEAL << "║" << RESET << "\n";
        std::cout << TEAL << "╚══════════════════════════════════╝" << RESET << "\n\n";
    }
    m_xWasPressed = xNow;

    // P: pausar / reanudar animaciones
    bool pNow = glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS;
    if (pNow && !m_pWasPressed)
    {
        m_ufoPaused = !m_ufoPaused;
        std::cout << TEAL << "╔══════════════════════════════════╗" << RESET << "\n";
        if (m_ufoPaused)
            std::cout << TEAL << "║" << RESET << "      UFO: Animación pausada      " << TEAL << "║" << RESET << "\n";
        else
            std::cout << TEAL << "║" << RESET << "     UFO: Animación reanudada     " << TEAL << "║" << RESET << "\n";
        std::cout << TEAL << "╚══════════════════════════════════╝" << RESET << "\n\n";
    }
    m_pWasPressed = pNow;

    // 1/2/3: modo de render
    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS && m_renderMode != 0)
    {
        m_renderMode = 0;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "         Modo: Superficie         " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n\n";
    }
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS && m_renderMode != 1)
    {
        m_renderMode = 1;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "         Modo: Wireframe          " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n\n";
    }
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS && m_renderMode != 2)
    {
        m_renderMode = 2;
        std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
        std::cout << TEAL << "\u2551" << RESET << "       Modo: Nube de Puntos       " << TEAL << "\u2551" << RESET << "\n";
        std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n\n";
    }

    if (m_camera->isFreeMode())
    {
        float moveSpeed = 30.0f * deltaTime;
        float fwd = 0, rgt = 0, upd = 0;

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) fwd += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) fwd -= moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) rgt -= moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) rgt += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) upd += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) upd -= moveSpeed;

        if (fwd != 0 || rgt != 0 || upd != 0)
            m_camera->move(fwd, rgt, upd);

        Vector3D pos = m_camera->getPosition();
        if (pos.y < CAMERA_MIN_Y)
            m_camera->setPosition({pos.x, CAMERA_MIN_Y, pos.z});

        // Flechas: rotar vista
        float lookSpeed = 2.0f * deltaTime;
        float dYaw = 0, dPitch = 0;
        if (glfwGetKey(win, GLFW_KEY_LEFT)  == GLFW_PRESS) dYaw   += lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) dYaw   -= lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_UP)    == GLFW_PRESS) dPitch -= lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS) dPitch += lookSpeed;
        if (dYaw != 0 || dPitch != 0)
            m_camera->orbit(dYaw, dPitch);
    }
    else
    {
        // Orbital: WASD orbita alrededor del centro
        float orbitSpeed = 2.0f * deltaTime;
        float dYaw = 0, dPitch = 0;
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) dYaw   -= orbitSpeed;
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) dYaw   += orbitSpeed;
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) dPitch += orbitSpeed;
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) dPitch -= orbitSpeed;
        if (dYaw != 0 || dPitch != 0)
            m_camera->orbit(dYaw, dPitch);

        float zoomSpeed = 40.0f * deltaTime;
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) m_camera->zoom(-zoomSpeed);
        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) m_camera->zoom( zoomSpeed);

        // Flechas: orbitar vista
        float arrowSpeed = 2.0f * deltaTime;
        float aYaw = 0, aPitch = 0;
        if (glfwGetKey(win, GLFW_KEY_LEFT)  == GLFW_PRESS) aYaw   += arrowSpeed;
        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) aYaw   -= arrowSpeed;
        if (glfwGetKey(win, GLFW_KEY_UP)    == GLFW_PRESS) aPitch += arrowSpeed;
        if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS) aPitch -= arrowSpeed;
        if (aYaw != 0 || aPitch != 0)
            m_camera->orbit(aYaw, aPitch);

        m_camera->clampOrbitalMinY(CAMERA_MIN_Y);
    }
}

// Uniforms de iluminación
void Scene::setLightUniforms(ShaderProgram *sh)
{
    sh->use();
    sh->setVec3("viewPos",      m_camera->getPosition());
    sh->setInt("lightsOn",      m_lightsOn  ? 1 : 0);
    sh->setInt("shadersOn",     m_shadersOn ? 1 : 0);

    // Material por defecto (piedra/concreto)
    sh->setFloat("shininess",   10.0f);
    sh->setVec3("matSpecular",  {0.05f, 0.05f, 0.05f});
    sh->setFloat("matDiffuse",  1.0f);

    // Spotlight (luz 2 — UFO apunta hacia abajo)
    sh->setVec3("spotDir",      {0.0f, -1.0f, 0.0f});
    sh->setFloat("spotCutOuter", std::cos(20.0f * PI / 180.0f));
    sh->setFloat("spotCutInner", std::cos( 8.0f * PI / 180.0f));

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        sh->setVec3("lightPos["  + std::to_string(i) + "]", m_lightPos[i]);
        sh->setVec3("lightAmb["  + std::to_string(i) + "]", m_lightAmbient[i]);
        sh->setVec3("lightDif["  + std::to_string(i) + "]", m_lightDiffuse[i]);
        sh->setVec3("lightSpec[" + std::to_string(i) + "]", m_lightSpecular[i]);
    }
}

// Bucle principal
void Scene::render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    double lastFrame = glfwGetTime();

    while (!m_window->shouldClose())
    {
        // Delta Time: tiempo entre frames
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;

        processInput(deltaTime);

        glClearColor(0.53f, 0.81f, 0.98f, 1.f); // sky blue
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix4D view       = m_camera->getViewMatrix();
        Matrix4D projection = m_camera->getProjectionMatrix(m_window->getAspectRatio());

        // 0. Skybox
        m_skybox->render(view, projection);

        // 1. Uniforms de iluminación
        setLightUniforms(m_shader);
        setLightUniforms(m_normalShader);

        // 2. Habitación
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (m_room) m_room->render(view, projection);

        // 3. Modelos OBJ
        switch (m_renderMode)
        {
        case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  break;
        case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); glPointSize(2.f); break;
        default: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
        }

        for (auto *model : m_models)
        {
            std::string lower = model->getName();
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

            if (lower.find("ufo") != std::string::npos)
            {
                // Máquina de estados UFO
                if (!m_ufoPaused)
                {
                m_ufoTimer += deltaTime;

                switch (m_ufoPhase)
                {
                case UFOPhase::Waiting:
                    m_ufoVisible = false;
                    if (m_ufoTimer >= m_ufoWaitTime)
                    {
                        m_ufoTimer = 0.0f;
                        m_ufoY     = m_ufoStartY;
                        m_ufoPhase = UFOPhase::Descending;
                        m_ufoVisible = true;
                        std::cout << TEAL << "  [UFO] Aparece — iniciando descenso.\n" << RESET;
                    }
                    break;

                case UFOPhase::Descending:
                    m_ufoVisible = true;
                    m_ufoY -= m_ufoSpeed * deltaTime;
                    if (m_ufoY <= m_ufoLandY)
                    {
                        m_ufoY     = m_ufoLandY;
                        m_ufoTimer = 0.0f;
                        m_ufoPhase = UFOPhase::Staying;
                        std::cout << TEAL << "  [UFO] Aterrizaje — permanece " << (int)m_ufoStayTime << "s.\n" << RESET;
                    }
                    break;

                case UFOPhase::Staying:
                    m_ufoVisible = true;
                    if (m_ufoTimer >= m_ufoStayTime)
                    {
                        m_ufoTimer = 0.0f;
                        m_ufoPhase = UFOPhase::Ascending;
                        std::cout << TEAL << "  [UFO] Iniciando ascenso.\n" << RESET;
                    }
                    break;

                case UFOPhase::Ascending:
                    m_ufoVisible = true;
                    m_ufoY += m_ufoSpeed * deltaTime;
                    if (m_ufoY >= m_ufoStartY)
                    {
                        m_ufoY       = m_ufoStartY;
                        m_ufoTimer   = 0.0f;
                        m_ufoPhase   = UFOPhase::Waiting;
                        m_ufoVisible = false;
                        std::cout << TEAL << "  [UFO] Desaparece — espera " << (int)m_ufoWaitTime << "s.\n" << RESET;
                    }
                    break;
                }
                }

                // Spotlight: activa solo cuando la nave es visible
                if (m_ufoVisible)
                {
                    m_lightPos[2]      = Vector3D{0.0f, m_ufoY, 0.0f};
                    m_lightDiffuse[2]  = Vector3D{0.03f, 0.46f, 0.06f};
                    m_lightSpecular[2] = Vector3D{0.02f, 0.30f, 0.04f};
                }
                else
                {
                    // Apagar spotlight
                    m_lightDiffuse[2]  = Vector3D{0.0f, 0.0f, 0.0f};
                    m_lightSpecular[2] = Vector3D{0.0f, 0.0f, 0.0f};
                    m_lightAmbient[2]  = Vector3D{0.0f, 0.0f, 0.0f};
                    continue;
                }

                // Actualizar uniforms de luz con la nueva posición del spotlight
                setLightUniforms(m_shader);
                setLightUniforms(m_normalShader);

                Vector3D uc = model->getCenter();
                float ur = model->getBoundingRadius();
                if (ur < 1e-6f) ur = 1.0f;
                float s = 25.0f / ur;

                // Rotación lenta sobre Y
                float spin = static_cast<float>(currentFrame) * 15.0f * PI / 180.0f;

                Matrix4D mat =
                    Matrix4D::translate({0.0f, m_ufoY, 0.0f})
                    * Matrix4D::rotateY(spin)
                    * Matrix4D::scale({s, s, s})
                    * Matrix4D::translate({-uc.x, -uc.y, -uc.z});
                model->setModelMatrix(mat);

                // Renderizar UFO con material metálico
                m_shader->use();
                m_shader->setFloat("shininess",   128.0f);
                m_shader->setVec3("matSpecular",  {1.0f, 1.0f, 1.0f});
                m_shader->setFloat("matDiffuse",  0.7f);
                m_normalShader->use();
                m_normalShader->setFloat("shininess",  128.0f);
                m_normalShader->setVec3("matSpecular", {1.0f, 1.0f, 1.0f});
                m_normalShader->setInt("useNormalMap", 1);
                model->renderNormal(m_normalShader, view, projection);
                continue;
            }

            if (lower.find("spaceship") != std::string::npos ||
                lower.find("duck") != std::string::npos)
            {
                // La nave pequeña orbita alrededor del Espacio Escultórico
                if (!m_ufoPaused)
                    m_birdAngle += 10.0f * deltaTime;

                Vector3D bc = model->getCenter();
                float br = model->getBoundingRadius();
                if (br < 1e-6f) br = 1.0f;
                float s   = 6.0f / br;
                float rad = m_birdAngle * PI / 180.0f;
                const float orbitR = 100.0f;

                Matrix4D mat =
                    Matrix4D::translate({orbitR * std::cos(rad), 35.0f, orbitR * std::sin(rad)})
                    * Matrix4D::rotateY(rad + PI * 0.5f)
                    * Matrix4D::scale({s, s, s})
                    * Matrix4D::rotateX(-PI * 0.5f)
                    * Matrix4D::translate({-bc.x, -bc.y, -bc.z});
                model->setModelMatrix(mat);
            }

            // Material por tipo de modelo
            if (lower.find("cat") != std::string::npos ||
                lower.find("banana") != std::string::npos)
            {
                // Banana-Cat: 4 instancias en los bordes del mundo
                Vector3D cc = model->getCenter();
                float cr = model->getBoundingRadius();
                if (cr < 1e-6f) cr = 1.0f;
                float s = 1.5f / cr;

                struct { float x, y, z, ry; } positions[] = {
                    {  0.f,  1.f, -51.f,  0.f       }, // norte → mira +Z
                    { 51.f,  1.f,   0.f, -PI * 0.5f }, // este  → mira -X
                    {  0.f,  1.f,  51.f,  PI        }, // sur   → mira -Z
                    {-51.f,  1.f,   0.f,  PI * 0.5f }, // oeste → mira +X
                };

                m_shader->use();
                m_shader->setFloat("shininess",  24.0f);
                m_shader->setVec3("matSpecular", {0.15f, 0.15f, 0.15f});
                m_shader->setFloat("matDiffuse", 1.0f);

                for (auto &p : positions)
                {
                    Matrix4D mat =
                        Matrix4D::translate({p.x, p.y, p.z})
                        * Matrix4D::rotateY(p.ry)
                        * Matrix4D::scale({s, s, s})
                        * Matrix4D::rotateX(-PI * 0.5f)
                        * Matrix4D::translate({-cc.x, -cc.y, -cc.z});
                    model->setModelMatrix(mat);
                    model->render(view, projection);
                }
                continue;
            }
            else if (lower.find("espacio") != std::string::npos ||
                lower.find("escultorico") != std::string::npos)
            {
                // Concreto / roca volcánica: muy opaco, sin brillo
                m_shader->use();
                m_shader->setFloat("shininess",  10.0f);
                m_shader->setVec3("matSpecular", {0.05f, 0.05f, 0.05f});
                m_shader->setFloat("matDiffuse", 1.0f);
            }
            else
            {
                // Default para animales / baliza: semi-máte
                m_shader->use();
                m_shader->setFloat("shininess",  24.0f);
                m_shader->setVec3("matSpecular", {0.15f, 0.15f, 0.15f});
                m_shader->setFloat("matDiffuse", 1.0f);
            }

            model->render(view, projection);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (m_axesOn)
        {
            glDisable(GL_DEPTH_TEST);
            m_axes->render(view, projection);
            glEnable(GL_DEPTH_TEST);
        }

        m_window->swapBuffers();
        m_window->pollEvents();
    }

    std::cout << TEAL << "\nSesion terminada\n\n" << RESET;
}