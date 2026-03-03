#ifndef DIRECTMOVEMENT_H
#define DIRECTMOVEMENT_H

#include "IMovementBehavior.h"

/**
 * @class DirectMovement
 * @brief Implementación de movimiento lineal constante.
 * * Esta clase mueve al objeto en línea recta hacia el objetivo a una 
 * velocidad uniforme, sin importar la distancia a la que se encuentre.
 */
class DirectMovement: public IMovementBehavior{

private: 
    /**
     * @brief Vector de dirección calculado en cada frame.
     * Guardarlo como miembro privado evita recrear la variable en cada llamada 
     * a Move
     */
    math::linear::Vector2D direction{};

public: 
    /**
     * @brief Calcula el desplazamiento lineal hacia el objetivo.
     * 1. Calcular el vector diferencia y realizar el desplazamiento (visto en clase)
     * * @override -> Significa que Implementa el método puro (=0) de IMovementBehavior.
     */
    void Move(  math::linear::Vector2D& pos, 
                const math::linear::Vector2D& target, 
                float speed, 
                float dt) override; 
                // Tu codigo va aquí :)  o en un .cpp 

};

#endif