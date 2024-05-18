#include <iostream>
#include <random>
#include <chrono>
#include "Audio.h"
#include <SFML/Graphics.hpp>
#include "Chip.hpp"
#include "ChipManager.hpp"
#include <filesystem>
#include "FileSystem.h"

void defaultRun()
{
	std::string pongGamePath = "./programs/pong2.c8";
	std::string tetrisGamePath = "./programs/TETRIS";
	std::string testROM_1_Path = "./programs/1chip8logo.ch8";

	Chip chip;
	chip.init();
	chip.loadProgram(testROM_1_Path);

	ChipManager chipManager(chip);
	chipManager.run();
}

void menuRun()
{
    //current directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current directory: " << currentPath << std::endl;

    std::filesystem::path programsPath = currentPath / "programs";
    std::cout << "\"Programs\" directory: " << programsPath << std::endl;

    std::vector<std::string> programContents = FileSystem::getDirectoryContents(programsPath.string());

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu Example");


    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        //return 1;
    }

    // create menu items
    std::vector<sf::Text> menuItems;
    for (const auto& item : programContents) 
    {
        sf::Text menuItem(item, font, 24);
        menuItem.setPosition(100, 100 + 50 * menuItems.size()); // Adjust the position based on the index
        menuItems.push_back(menuItem);
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red); // Highlight the initially selected item

    // Game path
    std::string gamePath;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (selectedItemIndex > 0)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White); // Un-highlight the previously selected item
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red); // Highlight the new selected item
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White); // Un-highlight the previously selected item
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red); // Highlight the new selected item
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    gamePath = "./programs/" + programContents[selectedItemIndex];
                    std::cout << "Selected game path: " << gamePath << std::endl;

                    window.close();
                    Chip chip;
                    chip.init();
                    chip.loadProgram(gamePath);

                    ChipManager chipManager(chip);
                    chipManager.run();

                    
                }
            }
        }

        // Clear the window with a color other than white
        window.clear(sf::Color::Black); // Change the color to something visible, like black

        // Draw menu items
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }

        // Display the window
        window.display();
    }
}

int main()
{

    menuRun();
	//defaultRun();
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