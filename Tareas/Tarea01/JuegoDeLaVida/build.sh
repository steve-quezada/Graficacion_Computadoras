#!/bin/bash

# Script de compilación y ejecución para Linux/macOS
# Usa una carpeta de build en base a un argumento opcional

echo ""
echo -e "\033[38;2;0;128;128m╔══════════════════════╗\033[0m"
echo -e "\033[38;2;0;128;128m║   Juego de la Vida   ║\033[0m"
echo -e "\033[38;2;0;128;128m╚══════════════════════╝\033[0m"
echo ""

OS="${1:-linux}"
if [ "$OS" != "linux" ] && [ "$OS" != "macos" ]; then
    echo -e "\033[31m[ERROR]\033[0m Uso: ./build.sh [linux|macos]"
    exit 1
fi

# Determinar carpeta de build según argumento
BUILD_DIR="build-$OS"

echo -e "\033[34m[INFO]\033[0m Carpeta de build: $BUILD_DIR"
echo ""

# Limpiar y recrear directorio de build
echo -e "\033[38;2;0;128;128m[FASE 1]\033[0m Preparando directorio de build..."
if [ -d "$BUILD_DIR" ]; then
    echo -e "  → Limpiando directorio anterior..."
    rm -rf "$BUILD_DIR"
fi

echo -e "  → Creando directorio de build..."
mkdir -p "$BUILD_DIR"
echo -e "  ✓ Directorio listo\033[0m"
echo ""

# Configurar con CMake
echo -e "\033[38;2;0;128;128m[FASE 2]\033[0m Configurando CMake..."
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo -e "  ✗ Error en CMake configure\033[0m"
    exit 1
fi
echo -e "  ✓ Configuración completada\033[0m"
echo ""

# Compilar
echo -e "\033[38;2;0;128;128m[FASE 3]\033[0m Compilando..."
cmake --build "$BUILD_DIR"
if [ $? -ne 0 ]; then
    echo -e "  ✗ Error en compilación\033[0m"
    exit 1
fi
echo -e "  ✓ Compilación exitosa\033[0m"
echo ""

# Ejecutar
echo -e "\033[38;2;0;128;128m[FASE 4]\033[0m Ejecutando..."
echo ""
"./$BUILD_DIR/juego"
