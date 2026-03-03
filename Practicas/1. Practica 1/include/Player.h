#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player: public Entity{

public: 

    using Entity::Entity; // Reusa el constructor

    void Update(float deltaTime) override;

};

#endif 