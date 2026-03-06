#include "Game.h"

#include <random>

void static OneEnemy(){
    
    // Instancia de jugador 
    Player p1{sf::Color(0, 128, 128), 15, 400.0f, 300.0f, 100.0f};
    
    // Distancia respecto al jugador
    math::linear::Vector2D enemy_offset {30.f, 30.f};
    math::linear::Vector2D enemy_1_initial_pos {p1.getPosition()+enemy_offset} ;
    // Instancia de enemigo
    Enemy enemy_1{sf::Color(100, 0, 150), 15, enemy_1_initial_pos[0], enemy_1_initial_pos[1], 2.0f, p1.getPosition()};

    std::vector<Enemy> enemies {enemy_1};
    enemies[0].ChangeMovement(enemies[0].directMovement);
        
    Game game{p1, enemies};

    game.Update();   
}

void static TwoEnemies(){
    Player p1{sf::Color(0, 128, 128), 15, 400.0f, 300.0f, 100.0f};

    math::linear::Vector2D offset1{50.f, 50.f};
    math::linear::Vector2D pos1{p1.getPosition() + offset1};
    Enemy enemy_1{sf::Color(100, 0, 150), 15, pos1[0], pos1[1], 50.0f, p1.getPosition()};

    math::linear::Vector2D offset2{-50.f, -50.f};
    math::linear::Vector2D pos2{p1.getPosition() + offset2};
    Enemy enemy_2{sf::Color(148, 0, 211), 15, pos2[0], pos2[1], 1.0f, p1.getPosition()};

    std::vector<Enemy> enemies{enemy_1, enemy_2};
    enemies[0].ChangeMovement(enemies[0].directMovement);
    enemies[1].ChangeMovement(enemies[1].lerpMovement);

    Game game{p1, enemies};
    game.Update();
}

int main(){

    OneEnemy();

    // TwoEnemies();
}
