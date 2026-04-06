#include "Player.h"

void Player::Update(float deltaTime){
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;

    // 2. Capturar Input del teclado
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_direction[1] -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_direction[1] += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_direction[0] -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_direction[0] += 1.0f;

    ComputeMovement(deltaTime);
}