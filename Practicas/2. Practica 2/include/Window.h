// Inicializa GLFW-GLEW y crea la ventana

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    GLFWwindow *handle; // Puntero que representa la ventana

    Window(int width, int height, const char *title);
    bool shouldClose();                               
    void swapBuffers();                               
    void pollEvents();                                
    ~Window();                                        
};
