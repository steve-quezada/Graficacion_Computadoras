// Inicializa GLFW-GLEW y crea la ventana

#include "Window.h"
#include <iostream>

// Ajusta la vista al redimensionar la ventana [main.cpp]
static void framebufferSizeCallback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Inicializacion y configuracion [main.cpp]
Window::Window(int width, int height, const char *title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (handle == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(handle);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    }
}

// Pregunta si se cerró la ventana
bool Window::shouldClose()
{
    return glfwWindowShouldClose(handle);
}

// Muestra el frame que se acaba de dibujar
void Window::swapBuffers()
{
    glfwSwapBuffers(handle);
}

// Procesa los eventos del sistema
void Window::pollEvents()
{
    glfwPollEvents();
}

// Libera todos los recursos
Window::~Window()
{
    glfwTerminate();
}
