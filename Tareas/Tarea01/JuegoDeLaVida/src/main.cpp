#include <SFML/Graphics.hpp>
#include "Celula.h"
#include <vector>
#include <typeinfo>
#include <iostream>
#include <chrono>
#include <thread>

void calculaEstadosFuturos(const std::vector<Celula*>& celulas);

int main()
{
	// Se crea la ventana de la aplicación
	sf::RenderWindow window( sf::VideoMode( { 504, 504 } ), "Juego de la vida" );

	// Ciclo en el que se crean los objetos
	float pos_x = 0.f;
	float pos_y = 0.f;

	// Lista que almacena las celulas
	std::vector<Celula*> celulas;

	// Se crean las celulas y se guardan apuntadores de las mismas
	for (int i = 0; i<5; i++){
		for (int j = 0; j<5;j++)
		{
			Celula* celula = new Celula(pos_x,pos_y);
			pos_x += 101.f;
			celulas.push_back(celula);
		}
		pos_x = 0.f;
		pos_y += 101.f;
	}

	// El color de fondo a usar
	sf::Color color(203,224,233);

	// GAME LOOP
	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
			//Se revisa cuando se da un click
			else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				// Se obtiene la coordenada del click
				int mouse_x = mouseButtonPressed->position.x;
        		int mouse_y = mouseButtonPressed->position.y;

				//Se obtienen los indices correspondientes del arreglo de cuadrados
				int coord_x = mouse_x/101;
				int coord_y = mouse_y/101;

				// Se convierte el indice a una dimension
				int index = (coord_y*5+coord_x);

				celulas.at(index)->cambiaEstado();
				// Se redibujan las Celulas
				window.clear(color);
				for (Celula* celula : celulas)
				{
					window.draw(celula->getCuadrado());
				}
				window.display();
			}
			//Se revisa cuando se presiona la tecla ENter para comenzar la simulación
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Enter)
				{
					// número de iteraciones a realizar 
					int iteraciones = 24;
					int contador = 0;

					while(contador++ <iteraciones)
					{
						// Se verifica si todas las celulas estan muertas para temrinar la simulación.
						bool muertos = false;
						for (Celula* celula: celulas)
						{
							if (celula->isViva())
								muertos = true;
						}
						if (!muertos)
							break;
			
						// Comienza la simulación
						std::cout<<"Iteración : "<<contador<<std::endl;
						calculaEstadosFuturos(celulas); // Se calculan los nuevos estados
						// Se actualizan los estados de la celula
						for (Celula* celula: celulas)
						{
							if (celula->isViva() != celula->getEstadoFuturo())
							{
								celula->cambiaEstado();
								celula->setEstadoFuturo(false);
							}
						}
						// Se redibujan las celulas
						window.clear(color);
						for (Celula* celula : celulas)
						{
							window.draw(celula->getCuadrado());
						}
						window.display();
						// Se duerme el hilo medio segundo
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
					}
				}
			}
		}

		window.clear(color);
		for (Celula* celula : celulas)
		{
			window.draw(celula->getCuadrado());
		}
		window.display();
	}
}

/**
 * @brief función que asigna los estados futuros a las células. 
 * Esta función debe verificar el número de vecinos vivos por cada celula y actaulizar su estado futuro
 * de acuerdo a las reglas del juego de la vida
 */
void calculaEstadosFuturos(const std::vector<Celula*>& celulas)
{
	//TODO
}
