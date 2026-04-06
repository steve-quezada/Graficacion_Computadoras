#ifndef IMOVEMENTBEHAVIOR_H
#define IMOVEMENTBEHAVIOR_H

#include "Vector2D.h"

/**
 * @class IMovementBehavior
 * @brief Interfaz base para definir estrategias de movimiento (Pattern Strategy).
 * Esta es una "Clase Abstracta Pura". Su función es definir el comportamiento 
 * de cualquier movimiento que la implemente.  
 * No se puede instanciar directamente.
 */
class IMovementBehavior{

public: 

    /**
     * @brief Constructor por defecto
     */
    IMovementBehavior()=default;

    /**
     * @brief Destructor virtual 
     */
    virtual ~IMovementBehavior()=default;


    /**
     * @brief Método de movimiento polimórfico.
     * Todas las clases derivadas deben implementar esta función.
     * @param currentPos Referencia a la posición actual del Enemy.h
     * @param target Posición del objetivo a alcanzar (El Player.h)
     * @param speed rapidez de movimiento
     * @param dt tiempo transcurrido
     */
    virtual void Move(math::linear::Vector2D& currentPos, 
                      const math::linear::Vector2D& target, 
                      float speed, float dt) = 0;

};

#endif