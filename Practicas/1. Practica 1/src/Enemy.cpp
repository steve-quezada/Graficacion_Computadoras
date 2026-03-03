#include "Enemy.h"

Enemy::Enemy(sf::Color color, float radius, float x, float y, float speed, math::linear::Vector2D& target)
    : Entity(color, radius, x, y, speed), // Inicializa la clase padre
      m_target(target)                 // Inicializa la referencia al target
{
}

void Enemy::ComputeMovement(float deltaTime){
    // Tu codigo va aquí :)     
}

void Enemy::Update(float deltaTime){
    
    // Tu codigo va aquí :)     
}

void Enemy::ChangeMovement(IMovementBehavior& newMovement){
    // Tu codigo va aquí :)     
}