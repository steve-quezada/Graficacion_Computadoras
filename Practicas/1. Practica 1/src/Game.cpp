#include "Game.h"

Game::Game(Player& player, std::vector<Enemy>& enemies)
    : m_player(player),
      m_deltaTime{0.0f},
      m_enemies(enemies)
    {

    m_window.create(sf::VideoMode(800, 600), "Practica 1");
    m_window.setFramerateLimit(60);

    }

void Game::Update(){

    // sf::Clock se encarga de medir el tiempo real que pasa entre frames
    sf::Clock clock;  

    while (m_window.isOpen())
    {   
        // Reiniciamos el reloj y guardamos el tiempo transcurrido en segundos
        sf::Time elapsed = clock.restart();
        m_deltaTime = elapsed.asSeconds(); // <-deltaTime, es el utilizado como "tiempo" para los movimientos 
        while (m_window.pollEvent(m_event))
        {
            if(m_event.type == sf::Event::Closed){
                m_window.close();
            }
        }
        
        // Cambio de movimiento con teclado
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
            for(auto& enemy : m_enemies){
                enemy.ChangeMovement(enemy.lerpMovement);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
            for(auto& enemy : m_enemies){
                enemy.ChangeMovement(enemy.directMovement);
            }
        }

        // -> Actualizacion jugador
        m_player.Update(m_deltaTime);

        // Actualizacion cada enemigo 
        for(auto& enemy : m_enemies){
            enemy.Update(m_deltaTime);
        }
        
        m_window.clear(sf::Color::Black);
        m_window.draw(m_player.getDrawable());

        // Renderizado por cada enemigo 
        for(auto& enemy : m_enemies){
            m_window.draw(enemy.getDrawable());
        }
        
        m_window.display();
    }
    
}