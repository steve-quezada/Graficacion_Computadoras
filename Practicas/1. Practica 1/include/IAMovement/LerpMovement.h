#ifndef LERPMOVEMENT_H
#define LERPMOVEMENT_H

#include "IMovementBehavior.h"

/**
 * @class LerpMovement
 * @brief Implementación de movimiento basado en Interpolación Lineal (Lerp).
 * El objeto recorre un porcentaje de la distancia restante en cada frame, lo que causa
 * que comience rápido y desacelere al llegar al objetivo.
 */
class LerpMovement: public IMovementBehavior{

    /**
     * @brief Realiza la interpolación entre la posición actual y el objetivo.
     * El factor 't' define qué tan cerca estamos del objetivo en este frame.
     * Si t = 0.1, nos movemos un 10% hacia el objetivo.
     * NOTA: A diferencia de DirectMovement, aquí no necesitamos normalizar vectores
     * porque la propia función Lerp gestiona la progresión entre puntos.
     */
    void Move( math::linear::Vector2D& pos, 
                const math::linear::Vector2D& target, 
                float speed, 
                float dt) override;
                // Tu codigo va aquí :)  o en un .cpp 

};

#endif