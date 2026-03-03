#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "IAMovement/IMovementBehavior.h"
#include "IAMovement/DirectMovement.h"
#include "IAMovement/LerpMovement.h"

/**
 * @class Enemy
 * @brief Representa a los agentes enemigos que persiguen a un objetivo.
 * Utiliza el IAMovement para cambiar entre tipo de comportamiento. 
 */
class Enemy : public Entity{

private: 

    math::linear::Vector2D& m_target; 

    // Buenas practicas ->  Inicializar un apuntador como nullptr 
    IMovementBehavior* m_currentMovement = nullptr; // Movimiento actual (puede cambiar)
    
public:

    LerpMovement lerpMovement {}; // Movimiento suave y desacelerado
    DirectMovement directMovement {}; // Movimiento lineal constante

    /**
     * @brief Constructor del enemigo.
     * @param target Referencia a la posición del jugador/objetivo.
     */
    Enemy(sf::Color color, float radius, float x, float y, float speed, math::linear::Vector2D& target);
    
    /**
     * @brief Decide y ejecuta la lógica de movimiento.
     * Aquí se evalúa qué comportamiento usar y se llama a Move() de IMovementBehavior
     * Es decir, se mueve dependiendo que comportamiento este configurado. 
     */
    void ComputeMovement(float deltaTime) override; 
    
    /**
     * @brief Actualización por frame.
     * Invoca a ComputeMovement y actualiza el estado general.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Cambia el tipo de movimiento dependiendo del Input
     * si se detecta sf::Keyboard::Num1 -> Usa LerpMovement 
     * si se detecta sf::Keyboard::Num2 -> Usa DirectMovment
     * @param newMovement Referencia al nuevo objeto comportamiento de movimiento.
     * Los movimientos de la clase Enemy son publicos, por lo que se pueden pasar como 
     * referencia al mismo metodo. 
     */
    void ChangeMovement(IMovementBehavior& newMovement);

};


#endif
