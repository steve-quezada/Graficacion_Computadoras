cmake_minimum_required(VERSION 3.24)
project(proyecto_final)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

# Windows (MinGW): usar finders personalizados de cmake/
if(WIN32)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)

if(WIN32)
    find_package(SOIL2 REQUIRED)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/Window.cpp
    src/engine/ShaderProgram.cpp
    src/engine/Camera.cpp
    src/models/Model.cpp
    src/models/CustomModel.cpp
    src/models/Axes.cpp
    src/models/Room.cpp
    src/models/SkyBox.cpp
    src/loaders/ObjLoader.cpp
    src/scene/Scene.cpp
)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "proyecto_final-win")
    target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw SOIL2::SOIL2)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_SOIL2 GLEW_STATIC)
    message(STATUS "Windows: proyecto_final-win.exe")
else()
    target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw soil2)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_SOIL2)
    message(STATUS "Linux/WSL: proyecto_final")
endif()

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
