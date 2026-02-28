# Author: @steve-quezada
# Script de compilación y ejecución para Windows con MinGW
# Detecta el SO y compila en carpeta específica

Write-Host "" 
$teal = "`e[38;2;0;128;128m"
$reset = "`e[0m"
Write-Host "${teal}╔═══════════════════════════════════╗${reset}"
Write-Host "${teal}║   Tarea 02 - Dibujo con Pixeles   ║${reset}"
Write-Host "${teal}╚═══════════════════════════════════╝${reset}"
Write-Host ""

# Determinar SO
$OS = "windows"
if ($PSVersionTable.Platform -eq "Unix") {
    $OS = "unix"
}

Write-Host "[INFO]" -ForegroundColor Blue -NoNewline; Write-Host " Sistema Operativo: $OS"

# Determinar carpeta de build según SO
$buildDir = "build-$OS"

Write-Host "[INFO]" -ForegroundColor Blue -NoNewline; Write-Host " Carpeta de build: $buildDir"

# Configuración de SFML (cambiar según tu instalación)
$SFML_DIR = "C:/SFML-3.0.2/lib/cmake/SFML"
Write-Host "[INFO]" -ForegroundColor Blue -NoNewline; Write-Host " SFML_DIR: $SFML_DIR"
Write-Host ""

# Limpiar y recrear directorio de build
Write-Host "${teal}[FASE 1]${reset}" -NoNewline; Write-Host " Preparando directorio de build..."
# Matar el proceso si sigue corriendo (libera el .exe bloqueado)
Get-Process dibujo -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 400
if (Test-Path $buildDir) {
    Write-Host "  → Limpiando directorio anterior..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir -ErrorAction SilentlyContinue
}

Write-Host "  → Creando directorio de build..." -ForegroundColor Yellow
New-Item -ItemType Directory -Path $buildDir | Out-Null
Write-Host "  ✓ Directorio listo" -ForegroundColor Green
Write-Host ""

# Configurar con CMake
Write-Host "${teal}[FASE 2]${reset}" -NoNewline; Write-Host " Configurando CMake..."
cmake -B $buildDir -G "MinGW Makefiles" -DSFML_DIR="$SFML_DIR" -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Error en CMake configure" -ForegroundColor Red
    exit 1
}
Write-Host "  ✓ Configuración completada" -ForegroundColor Green
Write-Host ""

# Compilar
Write-Host "${teal}[FASE 3]${reset}" -NoNewline; Write-Host " Compilando..."
cd $buildDir
mingw32-make
if ($LASTEXITCODE -ne 0) {
    Write-Host "  ✗ Error en compilación" -ForegroundColor Red
    cd ..
    exit 1
}
cd ..
Write-Host "  ✓ Compilación exitosa" -ForegroundColor Green
Write-Host ""

# Ejecutar
Write-Host "${teal}[FASE 4]${reset}" -NoNewline; Write-Host " Ejecutando..."
Write-Host ""
& ".\$buildDir\dibujo.exe"
