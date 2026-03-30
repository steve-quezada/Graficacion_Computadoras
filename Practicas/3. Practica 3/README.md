# Practica 3 — Cubo 3D 
<!-- Author: @steve-quezada -->

### Integrante
- Steve Quezada [@steve-quezada]

---

## Estructura del proyecto

```
3. Practica 3/
├── README.md                     ← Este archivo
├── CMakeLists.txt                ← Configuración CMake
│
├── include/                      ← Headers (.h)
│   ├── math/                     ←     Clases Matemáticas
│   │   ├── Vector3D.h            ←         Punto o dirección 3D (Operaciones Geométricas)
│   │   ├── Vector4D.h            ←         Vector3D con componente w (Coordenada Homogénea)
│   │   ├── Matrix3D.h            ←         Matriz 3×3 column-major (Operaciones Aritméticas)
│   │   └── Matrix4D.h            ←         Matriz 4×4 column-major (Transformaciones)
│   ├── engine/                   ←     OpenGL
│   │   ├── Window.h              ←         Ventana GLFW + contexto GLEW
│   │   ├── ShaderProgram.h       ←         Lectura, compilación y enlace de shaders
│   │   └── Camera.h              ←         Cámara orbital
│   ├── models/                   ←     Modelos 3D
│   │   ├── Model.h               ←         Clase base abstracta (VAO, VBO, EBO)
│   │   └── Cube.h                ←         Cubo
│   └── scene/                    ←     Orquestación
│       └── Scene.h               ←         Ciclo de render y entrada
│
├── src/                          ← Implementaciones (.cpp)
│   ├── main.cpp                  ←     Punto de entrada
│   ├── engine/
│   │   ├── Window.cpp
│   │   ├── ShaderProgram.cpp
│   │   └── Camera.cpp
│   ├── models/
│   │   ├── Model.cpp
│   │   └── Cube.cpp
│   └── scene/
│       └── Scene.cpp
│
├── shaders/                      ← Shaders GLSL
│   ├── vertex.vert               ←     Posición de vértices
│   └── fragment.frag             ←     Color interpolado
│
└── build/                        ← (Generado) Binarios
    └── practica3
```

---

## Compilación y Ejecución

```bash
# Configurar 
cmake -B build

# Compilar
cmake --build build

# Ejecutar
./build/practica3
```
---

## Flujo de ejecución

Al iniciar, `Scene` crea todos los recursos necesarios: la ventana, los shaders compilados en la GPU, el cubo con su geometría ya subida a la tarjeta gráfica, y la cámara colocada a cierta distancia mirando al centro.

Después entra en un ciclo que se repite mientras la ventana esté abierta. En cada vuelta lee el teclado para mover la cámara u cambiar el estado de la rotación, gira el cubo un poco si la rotación está activa, y le pide al cubo que se dibuje pasándole las matrices de vista y proyección. Al final intercambia los buffers para mostrar el frame en pantalla.

---

## Controles

| Tecla | Acción |
|---|---|
| `A` / `←` | Mover hacia la izquierda |
| `D` / `→` | Mover hacia la derecha |
| `W` / `↑` | Subir cámara |
| `S` / `↓` | Bajar cámara |
| `Espacio` | Pausar / reanudar rotación del cubo |
| `ESC` | Cerrar ventana |

---