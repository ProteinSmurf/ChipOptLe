#include <iostream>
#include <random>
#include <chrono>
#include "Audio.h"
#include <SFML/Graphics.hpp>
#include "Chip.hpp"
#include "ChipManager.hpp"

#include <filesystem>
#include "FileSystem.h"
#include "ChipApp.hpp"

void defaultRun()
{
	std::string pongGamePath = "./programs/pong2.c8";
	std::string tetrisGamePath = "./programs/TETRIS";
	std::string testROM_1_Path = "./programs/1chip8logo.ch8";

	Chip chip;
	chip.init();
	chip.loadProgram(pongGamePath);

	ChipManager chipManager(chip, 1);
	//chipManager.run();
}


int main()
{                         
    ChipApp app;
    app.initialize();
    app.run();

    /*sf::RenderWindow window(sf::VideoMode(800, 600), "Main Menu");
    mainMenu(window);*/

    //menuRun();
	//defaultRun();
	return 0;
}
