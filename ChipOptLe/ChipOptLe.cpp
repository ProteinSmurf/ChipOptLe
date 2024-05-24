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



int main()
{                         
    ChipApp app;
    app.initialize();
    app.run();
	return 0;
}
