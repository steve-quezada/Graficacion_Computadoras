cmake_minimum_required(VERSION 3.28)
project(juego LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags del compilador (equivalente a -Wall -Wextra)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

# Detectar sistema operativo
if(WIN32)
    message(STATUS "Configurando para Windows")
    # Forzar linkaje estatico en Windows
    set(SFML_STATIC_LIBRARIES TRUE)
    set(BUILD_SHARED_LIBS OFF)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()
elseif(APPLE)
    message(STATUS "Configurando para macOS")
else()
    message(STATUS "Configurando para Linux")
endif()

find_package(SFML 3 CONFIG REQUIRED COMPONENTS
    System
    Window
    Graphics
)

# Ejecutable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Celula.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
)

# Linkaje estatico completo (solo GNU/MinGW en Windows)
if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    target_link_options(${PROJECT_NAME} PRIVATE -static)
endif()