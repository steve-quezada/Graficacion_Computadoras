cmake_minimum_required(VERSION 3.24)
project(practica3)

# Estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags del compilador (equivalente a -Wall -Wextra)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Encontrar bibliotecas
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Ejecutable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/Window.cpp
    src/engine/ShaderProgram.cpp
    src/engine/Camera.cpp
    src/models/Model.cpp
    src/models/Cube.cpp
    src/scene/Scene.cpp
)

# Librerías
# Linkear las bibliotecas encontradas
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Copiar carpeta shaders al directorio del ejecutable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)