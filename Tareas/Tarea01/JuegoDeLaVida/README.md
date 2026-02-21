# Tarea 01 - Juego de la Vida [C++ + SFML + CMake]

### Integrante
- **Kevin Steve Quezada Ordoñez** (@steve-quezada)

---

## Descripción del Proyecto

El Juego de la vida es un autómata celular diseñado por el matemático británico John Horton Conway en 1970. Es un juego de cero jugadores, en el que su evolución es determinada por un estado inicial, sin requerir intervención adicional. Se considera un sistema Turing completo que puede simular cualquier otra Máquina de Turing.

El "tablero de juego" es una malla plana formada por cuadrados (las "células") que se extiende por el infinito en todas las direcciones. Por tanto, cada célula tiene 8 células "vecinas", que son las que están próximas a ella, incluidas las diagonales. Las células tienen dos estados: están "vivas" o "muertas" (o "encendidas" y "apagadas"). El estado de las células evoluciona a lo largo de unidades de tiempo discretas (se podría decir que por turnos). El estado de todas las células se tiene en cuenta para calcular el estado de las mismas al turno siguiente. Todas las células se actualizan simultáneamente en cada turno, siguiendo estas reglas:

---

## Reglas del Juego

El estado de las células evoluciona a lo largo de turnos. El estado de todas las células se tiene en cuenta para calcular el estado de las mismas al turno siguiente. Todas las células se actualizan simultáneamente en cada turno:

- **Nace**: Si una célula muerta tiene exactamente **3 células vecinas vivas**, nace (estará viva en el siguiente turno)
- **Muere por sobrepoblación**: Una célula viva muere si tiene **más de 3 vecinos vivos**
- **Muere por aislamiento**: Una célula viva muere si tiene **0 o 1 vecino vivo**
- **Vive**: Una célula viva se mantiene viva si tiene **2 o 3 vecinos vivos**

---

## Requisitos Previos

### Sistema Operativo
- **Windows 10/11** (con MinGW o MSVC)
- **Linux/Ubuntu** (GCC/Clang)
- **macOS** (Apple Clang)

### Software Requerido
| Herramienta | Versión | Instalación |
|-------------|---------|-------------|
| **CMake** | ≥ 3.28 | [cmake.org](https://cmake.org) |
| **Compilador C++** | C++20 | GCC, Clang, MSVC, MinGW |
| **SFML** | 3.0+ | [sfml-dev.org](https://www.sfml-dev.org) |
| **Git** | Cualquier | [git-scm.com](https://git-scm.com) |

---

## Estructura del Proyecto

```
JuegoDeLaVida/
├── README.md                          ← Este archivo
├── CMakeLists.txt                     ← Configuración CMake
├── build.ps1                          ← Script compilación Windows (PowerShell)
├── build.sh                           ← Script compilación Linux/macOS (Bash)
│
├── include/
│   └── Celula.h                       ← Encabezado clase Celula
│
├── src/
│   ├── main.cpp                       ← Programa principal (SFML + lógica)
│   └── Celula.cpp                     ← Implementación clase Celula
│
├── media/                             ← Recursos gráficos
│   ├── inicio.png
│   ├── click.png
│   └── ejemplo.gif
│
├── build-windows/                     ← (Generado) Binarios Windows
│   └── juego.exe
│
├── build-linux/                       ← (Generado) Binarios Linux
│   └── juego
│
└── build-macos/                       ← (Generado) Binarios macOS
    └── juego
```
---

## Compilación y Ejecución

### Opción 1: Scripts Automáticos

#### Windows (PowerShell)
```powershell
# Editar build.ps1 linea 20 si SFML está en otra ubicación
$SFML_DIR = "C:/SFML-3.0.2/lib/cmake/SFML"

# Ejecutar
.\build.ps1
```

#### Linux / macOS (Bash)
```bash
chmod +x build.sh
./build.sh linux

# macOS
./build.sh macos
```

### Opción 2: Compilación Manual con CMake

#### Windows (MinGW)
```powershell
mkdir build-windows
cd build-windows

# Configurar CMake con MinGW
cmake -G "MinGW Makefiles" -DSFML_DIR="C:/SFML-3.0.2/lib/cmake/SFML" -DCMAKE_BUILD_TYPE=Release ..

# Compilar
mingw32-make

# Ejecutar
.\juego.exe
```

#### Linux / macOS
```bash
mkdir build-linux
cd build-linux

cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

./juego
```