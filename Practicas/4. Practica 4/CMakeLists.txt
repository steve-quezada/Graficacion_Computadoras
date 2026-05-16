cmake_minimum_required(VERSION 3.24)
project(practica4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/Window.cpp
    src/engine/ShaderProgram.cpp
    src/engine/Camera.cpp
    src/models/Model.cpp
    src/models/CustomModel.cpp
    src/models/Axes.cpp
    src/models/Room.cpp
    src/loaders/ObjLoader.cpp
    src/scene/Scene.cpp
)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    GLEW::GLEW
    glfw
    soil2
)

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_SOIL2)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
