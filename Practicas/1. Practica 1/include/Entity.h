#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Vector2D.h"

/**
 * @class Entity
 * @brief Clase que modela a una entidad con posicion y representacion grafica 
 * utilizando la biblioteca SFML. Se mueve a partir de una direccion y velocidad 
 * a lo largo del tiempo. 
 */
class Entity{

protected: 

    sf::CircleShape m_shape; 
    math::linear::Vector2D m_position;
    math::linear::Vector2D m_direction;
    float m_speed;
    
public:

    /**
     * @brief Constructor de la entidad.
     * @param color Representacion como circulo.
     * @param radius Radio del circulo.
     * @param x,y Coordenadas iniciales.
     * @param speed Velocidad de movimiento base.
     */
    Entity(sf::Color color, float radius, float x, float y, float speed);
    
    /**
     * @brief Destructor virtual.
     * Es crucial que sea virtual para asegurar que las clases hijas se limpien 
     * correctamente de la memoria al usar polimorfismo.
     */
    ~Entity();

    /**
     * @brief Método puramente virtual (= 0). 
     * Convierte a Entity en una Clase Abstracta. Obliga a los hijos a implementar
     * su propia lógica de actualización frame a frame.
     */
    virtual void Update(float deltaTime) = 0; // Needs to be declared by children 

    /**
     * @brief Calcula la lógica de movimiento.
     * Se recomienda que se sobreescriba para la clase Enemy.h/Enemy.cpp
     * NOTA: La firma del metodo no puede cambiar 
     */
    virtual void ComputeMovement(float deltaTime);

    /**
     * @brief
     * Getter del Drawable para ser renderizado por una sf::RenderWindow
     */
    const sf::Drawable& getDrawable() const;
    

    // Retorna referencia para permitir modificaciones directas si es necesario
    math::linear::Vector2D& getPosition();
    
    // Versión const para lectura segura de la posición
    const math::linear::Vector2D& getPosition()const;

};


#endif