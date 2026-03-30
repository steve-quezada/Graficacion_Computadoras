// Implementación de la ventana OpenGL.

#include "engine/Window.h"
#include <iostream>

// Ajusta la vista al redimensionar la ventana
static void framebufferSizeCallback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char *title)
    : m_width{width}, m_height{height}
{
    // Configura la versión y perfil de OpenGL antes de crear la ventana
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crea la ventana y establece el contexto actual
    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (handle == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(handle);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

    // Inicializa GLEW para cargar las funciones de OpenGL
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    }
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(handle);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(handle);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

float Window::getAspectRatio() const
{
    return static_cast<float>(m_width) / static_cast<float>(m_height);
}

Window::~Window()
{
    glfwTerminate();
}
