#include "Enemy.h"

Enemy::Enemy(sf::Color color, float radius, float x, float y, float speed, math::linear::Vector2D& target)
    : Entity(color, radius, x, y, speed), // Inicializa la clase padre
      m_target(target)                 // Inicializa la referencia al target
{
}

void Enemy::ComputeMovement(float deltaTime){
    if(m_currentMovement != nullptr){
        m_currentMovement->Move(m_position, m_target, m_speed, deltaTime);
    }
}

void Enemy::Update(float deltaTime){
    ComputeMovement(deltaTime);
    m_shape.setPosition(m_position.x, m_position.y);
}

void Enemy::ChangeMovement(IMovementBehavior& newMovement){
    m_currentMovement = &newMovement;
}