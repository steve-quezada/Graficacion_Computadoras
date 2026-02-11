/**
 * @file Celula.h
 * @brief Clase que representa una célula en el juego de la vida
 */
#ifndef CELULA_H
#define CELULA_H
#include <SFML/Graphics.hpp>

class Celula
{
    private:
        /**
         * @brief El cuadrado a pintar
         */
        sf::RectangleShape m_cuadrado{{100.f,100.f}};

        /**
         * @brief El estado de la celula, true si está viva, false en otro caso.
         */
        bool m_estado{false};

        /**
         * @brief El estado al que transita la celula, true si pasará a estar viva, false en otro caso
         */
        bool m_estadoFuturo{false};

    public:

        /**
         * @brief Constructor para una celula, construye el cuadrado a ser dibujado
         * @param pos_x la posición en x del cuadrado
         * @param pos_y la posicion en y del cuadrado
         */
        Celula(float pos_x, float pos_y)
        {
            m_cuadrado.setPosition({pos_x,pos_y}); // Se define la posición del cuadrado
            m_cuadrado.setFillColor(sf::Color::Black); // Se define su color
        }

        /**
         * @brief Función que devuelve el cuadrado de la clase.
         * @returns La referencia del cuadrado.
         */
        sf::RectangleShape& getCuadrado();

        /**
         * @brief Cambia el estado de una celula, es decir si estaba viva pasa a estar muerta y viceversa
         * También modifica el color del cuadrado según el nuevo estado.
         */
        void cambiaEstado();

        /**
         * @brief Función que indica si la celula está viva o no.
         * @returns true si está viva, false en otro caso
         */
        bool isViva();

        /**
         * @brief Función que asigna a la celula su estado futuro.
         * @param estado el estado futuro de la celula 
         */
        void setEstadoFuturo(bool estado);

        /**
         * @brief Función que regresa el estado futuro de la celula
         * @returns true si el estaod futuro es vivo, false en otro caso
         */
        bool getEstadoFuturo();
};
#endif