# Practica 1
<!-- Author: @steve-quezada -->

### Integrante
- Steve Quezada (@steve-quezada)

---

## Formulas matematicas

Desplazamiento `DirectMovement`:
$$\vec{pos} \mathrel{+}= \hat{d} \cdot speed \cdot \Delta t, \quad \hat{d} = \frac{target - pos}{\|target - pos\|}$$

Interpolacion lineal `LerpMovement`:
$$pos = Lerp(pos,\, target,\, t), \quad t = speed \times \Delta t$$

$$Lerp(a, b, t) = (1-t)\,a + t\,b$$

---

## Estructura del proyecto

```
Practica 1/
├── README.md                     ← Este archivo
├── CMakeLists.txt                ← Configuración CMake
│
├── include/
│   ├── Entity.h                  ← Encabezado clase base abstracta
│   ├── Player.h                  ← Encabezado clase jugador
│   ├── Enemy.h                   ← Encabezado clase enemigo
│   ├── Vector2D.h                ← Struct 2D, operadores y función Lerp
│   └── IAMovement/
│       ├── IMovementBehavior.h   ← Comportamientos
│       ├── DirectMovement.h      ← Movimiento directo
│       └── LerpMovement.h        ← Movimiento por interpolación lineal
│
├── src/
│   ├── main.cpp                  ← Escenarios OneEnemy y TwoEnemies
│   ├── Entity.cpp                ← Posición y movimiento base
│   ├── Player.cpp                ← Captura input y dirección
│   ├── Enemy.cpp                 ← Persecución y cambio de comportamiento
│   └── Game.cpp                  ← Ventana, ciclo principal y manejo de input
│
└── build/                        ← (Generado) Binarios
    └── HandsOnClasses
```

---

## Compilacion y Ejecucion

```bash
# Configurar 
cmake -B build

# Compilar
cmake --build build

# Ejecutar
./build/HandsOnClasses
```

---

## Controles

| Tecla | Accion |
|-------|--------|
| `W A S D` | Mover al jugador |
| `Num1` | Cambiar a `LerpMovement` |
| `Num2` | Cambiar a `DirectMovement` |

---

### `DirectMovement` y `LerpMovement`

**¿Por qué `DirectMovement` puede parecer más lento?**

Considerando una línea entre el enemigo y el jugador. Las dos fórmulas las recorren de maneras distintas:

$$pos \mathrel{+}= \hat{d} \cdot speed \cdot \Delta t \quad \text{(DirectMovement)}$$

Avanza una cantidad fija por frame. No importa la longitud de la línea, el paso es siempre el mismo.

$$pos = Lerp(pos,\ target,\ speed \cdot \Delta t) \quad \text{(LerpMovement)}$$

Avanza un porcentaje de lo que falta por frame. Si queda mucho camino, da un paso grande. Si queda poco, da un paso pequeño.


**¿Por qué estos valores?**

Los valores se obtuvieron por experimentación. `speed` no tiene la misma unidad en los dos algoritmos, entonces, no existe un valor único que funcione para ambos.

Para `DirectMovement` se probaron valores hasta encontrar uno donde el movimiento fuera visible y no demasiado rápido en pantalla. `75.0f` resultó el mejor.

Para `LerpMovement` con `1.0f` el enemigo se acerca con una velocidad inicial rapida y frena al llegar.

Se confirmaron estos valores en `TwoEnemies`.

---

**Referencias**

- Seraphaestus. (2025). Whats the difference between lerp, and move towards? Obtenido de Reddit: https://www.reddit.com/r/godot/comments/1girt4q/whats_the_difference_between_lerp_and_move_towards/
- TheRoyalSkies. (8 de Marzo de 2021). Unity 3D USING Lerp vs MoveTowards vs SmoothDamp (In 2 Minutes). Obtenido de Patreon: https://www.patreon.com/posts/unity-3d-using-2-48478624?l=es

