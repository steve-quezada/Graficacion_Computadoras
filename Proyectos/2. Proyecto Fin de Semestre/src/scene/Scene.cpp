// Implementación de la escena.
// Cámara FPS con ratón, iluminación Phong, texturas SOIL2 y habitación explorable.

#include "scene/Scene.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <filesystem>
#include <algorithm>

static const float PI = 3.14159265f;

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
    delete m_room;
    delete m_axes;
    delete m_shader;
    delete m_axesShader;
    delete m_camera;
    delete m_window;
}

void Scene::init()
{
    m_window = new Window(1024, 768, "Practica 4 - Modelo de Phong y Texturas");
    m_shader = new ShaderProgram("shaders/vert/vertex.vert",
                                 "shaders/frag/fragment.frag");
    m_axesShader = new ShaderProgram("shaders/vert/axes.vert",
                                     "shaders/frag/axes.frag");

    // Cámara en x+,y+,z+ mirando al centro de la escena
    float fov = 60.0f * PI / 180.0f;
    m_camera = new Camera(
        Vector3D{0.0f, 0.0f, 0.0f}, // target: centro de la escena
        0.1f,                       // radius: distancia inicial al target
        0.78f,                      // pitch: ángulo vertical
        PI * 0.25f,                 // yaw: ángulo horizontal
        fov,                        // fov (radianes)
        0.05f,                      // near
        500.0f);                    // far
    m_camera->toggleFreeMode();
    m_camera->move(0, 0, 0);

    // Ejes escalados para referencia
    m_axes = new Axes(m_axesShader);
    m_axes->scale(8.0f);

    // Luz puntual
    m_lightPos = Vector3D{0.0f, 8.0f, 0.0f};
    m_lightColor = Vector3D{1.0f, 1.0f, 1.0f};

    m_renderMode = 0;

    std::cout << "\n";
    std::cout << TEAL << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557" << RESET << "\n";
    std::cout << TEAL << "\u2551" << RESET << "  Practica 4 - Modelo de Phong y Texturas  " << TEAL << "\u2551" << RESET << "\n";
    std::cout << TEAL << "\u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d" << RESET << "\n";
    std::cout << "\n";
    std::cout << TEAL << "[FASE 1]" << RESET << " Cargando modelos\n\n";

    loadModels();
    positionModels();

    std::cout << TEAL << "[FASE 2]" << RESET << " Iniciando ciclo de render\n\n";
    std::cout << "  Controles:\n";
    std::cout << "    W / S         Avanzar / Retroceder\n";
    std::cout << "    A / D         Mover Izquierda / Derecha\n";
    std::cout << "    Q / E         Subir / Bajar (libre) | Zoom (orbital)\n";
    std::cout << "    Flechas       Rotar Vista\n";
    std::cout << "    C             Alternar Camara (Libre / Orbital)\n";
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

    // Habitación
    m_room = new Room(m_shader, 10.0f);
    std::cout << "  " << "Texturas de Habitación cargadas (floor/ceil/wall)\n\n";

    std::vector<std::string> paths;
    for (const auto &entry : fs::directory_iterator(objDir))
    {
        if (entry.path().extension() == ".obj")
            paths.push_back(entry.path().string());
    }
    std::sort(paths.begin(), paths.end());

    for (const auto &path : paths)
    {
        std::string stem = fs::path(path).stem().string();
        std::string texPath = "assets/textures/" + stem + ".jpg";
        m_models.push_back(new CustomModel(m_shader, path.c_str(), texPath.c_str()));
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

        Matrix4D mat;

        if (name == "Whale.obj")
        {
            // Ballena: grande
            float s = 5.5f / radius;
            mat = Matrix4D::translate({0.0f, -2.5f, -3.0f}) * Matrix4D::scale({s, s, s}) * Matrix4D::rotateY(PI * 0.5f) * Matrix4D::rotateX(-PI * 0.5f) * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (name == "Beagle.obj")
        {
            // Beagle: mediano
            float s = 1.5f / radius;
            mat = Matrix4D::translate({-6.0f, -6.5f, 2.5f}) * Matrix4D::scale({s, s, s}) * Matrix4D::rotateY(PI * 0.5f) * Matrix4D::rotateX(-PI * 0.5f) * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (name == "Cat.obj")
        {
            // Gato: pequeño
            float s = 1.0f / radius;
            mat = Matrix4D::translate({5.5f, -9.0f, 1.5f}) * Matrix4D::scale({s, s, s}) * Matrix4D::rotateX(-PI * 0.5f) * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (name == "Bird.obj")
        {
            // Pajaro: pequeño
            float s = 0.7f / radius;
            mat = Matrix4D::translate({3.0f, 3.5f, -5.0f}) * Matrix4D::scale({s, s, s}) * Matrix4D::rotateX(-PI * 0.5f) * Matrix4D::translate({-center.x, -center.y, -center.z});
        }
        else if (name == "Toad.obj")
        {
            // Sapo: muy pequeño
            float s = 0.4f / radius;
            mat = Matrix4D::translate({-3.0f, -9.6f, 6.0f}) * Matrix4D::scale({s, s, s}) * Matrix4D::rotateX(-PI * 0.5f) * Matrix4D::translate({-center.x, -center.y, -center.z});
        }

        m->setModelMatrix(mat);
    }

    m_camera->setPosition({8.5f, 8.0f, 8.5f});
}

void Scene::processInput(float deltaTime)
{
    GLFWwindow *win = m_window->handle;

    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    // C: alternar cámara orbital / libre
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
        float moveSpeed = 12.0f * deltaTime;
        float fwd = 0, rgt = 0, upd = 0;

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) fwd += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) fwd -= moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) rgt -= moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) rgt += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) upd += moveSpeed;
        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) upd -= moveSpeed;

        if (fwd != 0 || rgt != 0 || upd != 0)
        {
            m_camera->move(fwd, rgt, upd);
            float h = m_room->getHalfSize() - 0.5f;
            Vector3D pos = m_camera->getPosition();
            if (pos.x >  h) pos.x =  h;
            if (pos.x < -h) pos.x = -h;
            if (pos.y >  h) pos.y =  h;
            if (pos.y < -h + 1.0f) pos.y = -h + 1.0f;
            if (pos.z >  h) pos.z =  h;
            if (pos.z < -h) pos.z = -h;
            m_camera->setPosition(pos);
        }

        // Flechas: rotar vista
        float lookSpeed = 2.0f * deltaTime;
        float dYaw = 0, dPitch = 0;
        if (glfwGetKey(win, GLFW_KEY_LEFT)  == GLFW_PRESS) dYaw   -= lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) dYaw   += lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_UP)    == GLFW_PRESS) dPitch += lookSpeed;
        if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS) dPitch -= lookSpeed;
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

        float zoomSpeed = 12.0f * deltaTime;
        if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) m_camera->zoom(-zoomSpeed);
        if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) m_camera->zoom( zoomSpeed);
    }
}

// Uniforms de luz y cámara
void Scene::setLightUniforms()
{
    m_shader->use();
    m_shader->setVec3("lightPos", m_lightPos);
    m_shader->setVec3("lightAmbient", m_lightColor * 0.1f);
    m_shader->setVec3("lightDiffuse", m_lightColor);
    m_shader->setVec3("lightSpecular", m_lightColor * 0.5f);
    m_shader->setVec3("viewPos", m_camera->getPosition());
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

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix4D view = m_camera->getViewMatrix();
        Matrix4D projection = m_camera->getProjectionMatrix(m_window->getAspectRatio());

        setLightUniforms();

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

        // la habitación siempre sólida
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_room->render(view, projection);

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

        for (auto *model : m_models)
        {
            if (model->getName() == "Bird.obj")
            {
                // El pajaro gira en el eje Y.
                m_birdAngle += 45.0f * deltaTime; // 45 grados por segundo

                Vector3D bc = model->getCenter();
                float br = model->getBoundingRadius();
                if (br < 1e-6f)
                    br = 1.0f;
                float s = 0.7f / br;
                float rad = m_birdAngle * PI / 180.0f;

                Matrix4D mat =
                    Matrix4D::translate({3.0f, 3.5f, -5.0f})      // 4. posicion en escena
                    * Matrix4D::rotateY(rad)                      // 3. rotacion mundo Y
                    * Matrix4D::scale({s, s, s})                  // 2. escalar
                    * Matrix4D::rotateX(-PI * 0.5f)               // 1b. correccion orientacion
                    * Matrix4D::translate({-bc.x, -bc.y, -bc.z}); // 1a. centrar
                model->setModelMatrix(mat);
            }
            else if (model->getName() == "Whale.obj")
            {
                // La ballena oscila en eje X con angulo entre -20 y +20 grados.
                Vector3D wc = model->getCenter();
                float wr = model->getBoundingRadius();
                if (wr < 1e-6f)
                    wr = 1.0f;
                float s = 5.5f / wr;
                float nod = 20.0f * std::sin((float)currentFrame * 1.5f) * PI / 180.0f;

                Matrix4D mat =
                    Matrix4D::translate({0.0f, -2.5f, -3.0f})     // 4. posicion en escena
                    * Matrix4D::rotateX(nod)                      // 3. cabeceo oscilante
                    * Matrix4D::scale({s, s, s})                  // 2. escalar
                    * Matrix4D::rotateY(PI * 0.5f)                // 1c. correccion orientacion
                    * Matrix4D::rotateX(-PI * 0.5f)               // 1b. correccion orientacion
                    * Matrix4D::translate({-wc.x, -wc.y, -wc.z}); // 1a. centrar
                model->setModelMatrix(mat);
            }
            model->render(view, projection);
        }

        // Restaurar modo de relleno
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_axes->render(view, projection);

        m_window->swapBuffers();
        m_window->pollEvents();
    }

    std::cout << TEAL << "\nSesion terminada\n\n"
              << RESET;

    glfwSetInputMode(m_window->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}