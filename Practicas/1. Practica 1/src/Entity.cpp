#include "Entity.h"

    Entity::Entity(sf::Color color, float radius, float x, float y, float speed)
        : m_speed{speed}{
            m_shape.setRadius(radius);
            m_shape.setOrigin(radius,radius);
            m_shape.setFillColor(color);
            m_position[0] = x;
            m_position[1] = y;
            m_shape.setPosition(x, y); 
        }

    Entity::~Entity(){
        
    }

    void Entity::ComputeMovement(float deltaTime){
        if(math::linear::Magnitude(m_direction) >0){
            // Modifica la posicion
            // // Tu codigo va aquí :)     
            // Se actualiza la representacion visual 
        }
    }

    const sf::Drawable& Entity::getDrawable() const {return m_shape;}

    math::linear::Vector2D& Entity::getPosition() {return m_position;}
    
    const math::linear::Vector2D& Entity::getPosition()const {return m_position;}

