// Define los vértices, crea los objetos y ejecuta el ciclo de renderizado
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "ShaderProgram.h"
#include "Polygon.h"

int main()
{
    Window window(800, 600, "MyFirstOpenGL_Cat");

    // Vértices del gato

    // Oreja Izquierda
    float oreja_izq[] = {
        -0.6542f,  0.9502f, 0.0f,
        -0.6532f,  0.4826f, 0.0f,
        -0.4303f,  0.7182f, 0.0f,
    };

    // Oreja Derecha
    float oreja_der[] = {
        -0.2062f,  0.9502f, 0.0f,
        -0.4303f,  0.7182f, 0.0f,
        -0.2061f,  0.4843f, 0.0f,
    };

    // Cabeza
    float cabeza[] = {
        -0.4303f,  0.7182f, 0.0f,
        -0.2061f,  0.4843f, 0.0f,
        -0.4274f,  0.2422f, 0.0f,
        
        -0.4303f,  0.7182f, 0.0f,
        -0.4274f,  0.2422f, 0.0f,
        -0.6532f,  0.4826f, 0.0f,
    };

    // Pecho
    float pecho[] = {
        -0.5855f,  0.3908f, 0.0f,
        -0.5853f, -0.2692f, 0.0f,
         0.0507f, -0.2647f, 0.0f,
    };

    // Cuerpo
    float cuerpo[] = {
        -0.5853f, -0.2692f, 0.0f,
         0.0507f, -0.2647f, 0.0f,
         0.0535f, -0.9347f, 0.0f,
    };

    // Pata
    float pata[] = {
        -0.2667f, -0.6061f, 0.0f,
        -0.5842f, -0.9390f, 0.0f,
         0.0535f, -0.9347f, 0.0f,
    };

    // Cola
    float cola[] = {
         0.0535f, -0.9347f, 0.0f,
         0.3790f, -0.9393f, 0.0f,
         0.6994f, -0.6050f, 0.0f,

         0.0535f, -0.9347f, 0.0f,
         0.6994f, -0.6050f, 0.0f,
         0.3787f, -0.6050f, 0.0f,
    };

    // Crear objetos Polygon
    Polygon polygon_oreja_izq(oreja_izq, 3);
    Polygon polygon_oreja_der(oreja_der, 3);
    Polygon polygon_cabeza(cabeza, 6);
    Polygon polygon_pecho(pecho, 3);
    Polygon polygon_cuerpo(cuerpo, 3);
    Polygon polygon_pata(pata, 3);
    Polygon polygon_cola(cola, 6);

    // Crear objetos ShaderProgram
    ShaderProgram shader_oreja_izq("shaders/vert/basic.vert", "shaders/frag/oreja_izq.frag");
    ShaderProgram shader_oreja_der("shaders/vert/basic.vert", "shaders/frag/oreja_der.frag");
    ShaderProgram shader_cabeza("shaders/vert/basic.vert", "shaders/frag/cabeza.frag");
    ShaderProgram shader_pecho("shaders/vert/basic.vert", "shaders/frag/pecho.frag");
    ShaderProgram shader_cuerpo("shaders/vert/basic.vert", "shaders/frag/cuerpo.frag");
    ShaderProgram shader_pata("shaders/vert/basic.vert", "shaders/frag/pata.frag");
    ShaderProgram shader_cola("shaders/vert/basic.vert", "shaders/frag/cola.frag");

    // Ciclo de renderizado [main.cpp]
    while (!window.shouldClose())
    {
        if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.handle, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar cada parte del gato con su shader
        shader_oreja_izq.use();
        polygon_oreja_izq.draw();

        shader_oreja_der.use();
        polygon_oreja_der.draw();

        shader_cabeza.use();
        polygon_cabeza.draw();

        shader_pecho.use();
        polygon_pecho.draw();

        shader_cuerpo.use();
        polygon_cuerpo.draw();

        shader_pata.use();
        polygon_pata.draw();

        shader_cola.use();
        polygon_cola.draw();

        window.swapBuffers();

        window.pollEvents();
    }

    return 0;
}