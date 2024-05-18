#include <iostream>
#include <random>
#include <chrono>
#include "Audio.h"
#include <SFML/Graphics.hpp>
#include "Chip.hpp"
#include "ChipManager.hpp"

int main()
{
	Chip chip;
	chip.init();
	chip.loadProgram("./programs/pong2.c8");

	ChipManager chipManager(chip);
	chipManager.run();

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