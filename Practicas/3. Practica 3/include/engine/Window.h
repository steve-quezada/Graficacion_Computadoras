// Crea y gestiona una ventana OpenGL con GLFW y GLEW.

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    GLFWwindow *handle;

    // Inicializa GLFW, crea la ventana con contexto OpenGL y GLEW
    Window(int width, int height, const char *title);

    // Devuelve true cuando el usuario solicita cerrar la ventana
    bool shouldClose();

    // Presenta el frame renderizado en pantalla
    void swapBuffers();

    // Procesa los eventos del sistema operativo
    void pollEvents();

    // Relación ancho/alto de la ventana
    float getAspectRatio() const;

    // Libera los recursos de GLFW
    ~Window();

private:
    int m_width, m_height;
};
