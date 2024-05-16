#include <iostream>
#include <random>
#include <chrono>
#include "Audio.h"
#include <SFML/Graphics.hpp>

int main()
{
	std::cout << "Hello World!\n";
	
	    sf::RenderWindow window(sf::VideoMode(640, 320), "Test window"); 

	    const int matrixWidth = 64;
	    const int matrixHeight = 32;
	    const float pixelSize = 10.0f; 
	

	    std::vector<std::vector<bool>> pixelStates(matrixWidth, std::vector<bool>(matrixHeight, false));

	    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	    std::uniform_int_distribution<int> distX(0, matrixWidth - 1);
	    std::uniform_int_distribution<int> distY(0, matrixHeight - 1);
	
	    sf::Clock clock;
	    int pixelsTurnedOn = 0;
	    bool needRedraw = false;
	
	    while (window.isOpen()) {
	        sf::Event event;
	        while (window.pollEvent(event)) {
	            if (event.type == sf::Event::Closed) {
	                window.close();
	            }
	        }
	
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	        {
	            window.close();
	        }
	
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	        {
	            Audio::playSound("beep.wav");
	        }

	        if (needRedraw) {
	            for (int x = 0; x < matrixWidth; x++) 
				{
	                for (int y = 0; y < matrixHeight; y++) 
					{
	                    pixelStates[x][y] = false;
	                }
	            }
	            needRedraw = false;
	        }

	        if (clock.getElapsedTime().asSeconds() >= 1.0f) 
			{
	            int randX = distX(rng);
	            int randY = distY(rng);
	            pixelStates[randX][randY] = true;
	            pixelsTurnedOn++;
	            if (pixelsTurnedOn >= 20) 
				{
	                needRedraw = true;
	                pixelsTurnedOn = 0;
	            }
	            clock.restart();
	        }
	
	        window.clear(sf::Color::Black);

	        for (int x = 0; x < matrixWidth; x++) 
			{
	            for (int y = 0; y < matrixHeight; ++y) 
				{
	                if (pixelStates[x][y]) 
					{
	                    sf::RectangleShape pixel(sf::Vector2f(pixelSize, pixelSize));
	                    pixel.setPosition(sf::Vector2f(x * pixelSize, y * pixelSize));
	                    pixel.setFillColor(sf::Color::White);
	                    window.draw(pixel);
	                }
	            }
	        }
	        window.display();
	    }
	
	    return 0;





}




// SFML website test code to check if it runs


//sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//sf::CircleShape shape(100.f);
//shape.setFillColor(sf::Color::Green);
//
//while (window.isOpen())
//{
//    sf::Event event;
//    while (window.pollEvent(event))
//    {
//        if (event.type == sf::Event::Closed)
//            window.close();
//    }
//
//    window.clear();
//    window.draw(shape);
//    window.display();
//}
//
//return 0;