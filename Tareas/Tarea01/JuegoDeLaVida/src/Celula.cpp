#include "Celula.h"

sf::RectangleShape& Celula::getCuadrado()
{
    return m_cuadrado;
}

void Celula::cambiaEstado()
{
    m_estado = !m_estado;
    m_cuadrado.setFillColor(m_estado ? sf::Color::White : sf::Color::Black);
}

bool Celula::isViva()
{
    return m_estado;
}

void Celula::setEstadoFuturo(bool estado)
{
    m_estadoFuturo = estado;
}

bool Celula::getEstadoFuturo()
{
    return m_estadoFuturo;
}