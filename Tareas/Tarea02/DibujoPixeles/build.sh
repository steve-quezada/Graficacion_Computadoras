#!/bin/bash
# Author: @steve-quezada
# Script de compilación y ejecución para Linux/macOS/WSL
# Detecta el SO y compila en carpeta específica

echo ""
teal="\033[38;2;0;128;128m"
reset="\033[0m"
echo -e "${teal}╔═══════════════════════════════════╗${reset}"
echo -e "${teal}║   Tarea 02 - Dibujo con Pixeles   ║${reset}"
echo -e "${teal}╚═══════════════════════════════════╝${reset}"
echo ""

# Detectar SO
if grep -qi "microsoft" /proc/version 2>/dev/null; then
    OS="wsl"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
else
    OS="linux"
fi

echo -e "\033[34m[INFO]\033[0m Sistema Operativo: $OS"

# Determinar carpeta de build según SO
BUILD_DIR="build-$OS"

echo -e "\033[34m[INFO]\033[0m Carpeta de build: $BUILD_DIR"
echo ""

# Limpiar y recrear directorio de build
echo -e "${teal}[FASE 1]${reset} Preparando directorio de build..."
if [ -d "$BUILD_DIR" ]; then
    echo -e "  → Limpiando directorio anterior..."
    rm -rf "$BUILD_DIR"
fi

echo -e "  → Creando directorio de build..."
mkdir -p "$BUILD_DIR"
echo -e "  ✓ Directorio listo\033[0m"
echo ""

# Configurar con CMake
echo -e "${teal}[FASE 2]${reset} Configurando CMake..."
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]; then
    echo -e "  ✗ Error en CMake configure\033[0m"
    exit 1
fi
echo -e "  ✓ Configuración completada\033[0m"
echo ""

# Compilar
echo -e "${teal}[FASE 3]${reset} Compilando..."
cmake --build "$BUILD_DIR"
if [ $? -ne 0 ]; then
    echo -e "  ✗ Error en compilación\033[0m"
    exit 1
fi
echo -e "  ✓ Compilación exitosa\033[0m"
echo ""

# Ejecutar
echo -e "${teal}[FASE 4]${reset} Ejecutando..."
echo ""
"./$BUILD_DIR/dibujo"
