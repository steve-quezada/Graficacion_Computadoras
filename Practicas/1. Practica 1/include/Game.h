#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"

/**
 * @class Game
 * @brief Clase controladora principal.
 * * Se encarga de gestionar la ventana de SFML, administrar el ciclo de vida 
 * de los objetos y controlar el tiempo de actualización (DeltaTime).
 */
class Game
{
private:

    sf::RenderWindow m_window;
    Player& m_player;
    std::vector<Enemy>& m_enemies; 
    sf::Event m_event;
    
    /**
     * @brief Tiempo transcurrido entre el frame anterior y el actual.
     * Se usa para que el movimiento sea independiente de la velocidad del procesador.
     */
    float m_deltaTime;

public:

    Game(Player& player, std::vector<Enemy>& enemies);

    void Update();

};

#endif