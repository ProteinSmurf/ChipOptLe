#include "ChipApp.hpp"
#include <iostream>
#include <filesystem>
#include "FileSystem.h"
#include "Chip.hpp"
#include "ChipManager.hpp"

ChipApp::ChipApp() 
{
    //nothing
}

void ChipApp::initialize() 
{
    std::cout << "Initializing ChipApp by setting default values to customization parameters" << std::endl;

    fontSize_ = 30;
    scaleFactor_ = 1;
    gameSpeed_ = 1000;
    backgroundColor_ = sf::Color::Black;
    foregroundColor_ = sf::Color::White;
    soundPath_ = "./sound/path";

    std::cout << "Scale Factor: " << scaleFactor_ << std::endl;
    std::cout << "Game Speed: " << gameSpeed_ << std::endl;
}

void ChipApp::run() 
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ChipOptLe");
    mainMenu(window);
}


void ChipApp:: mainMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Play", font, fontSize_);         // menuItems.emplace_back(<MenuOptionString>, font, fontSize_),
    menuItems.emplace_back("Options", font, fontSize_);



    /*
        sf::Text is a rectangle (width and height)

                            width
        |<--------------|  800 pixels |---------------->|
        |<--- 300px---->|             |<---- 300px----->|
        |-------------- |  MenuItem   |---------------->|

        MenuItem.width = 200px

    */

    //total height of menu items
    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    //  starting Y position at the middle of the screen vertically
    float startY = (window.getSize().y - totalHeight) / 2;
    //float startY = window.getSize().y / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20; //   vertical spacing between menu options 
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    { 
                        // Play
                        window.clear(sf::Color::Black);
                        window.display();
                        playMenu(window);
                    }
                    else if (selectedItemIndex == 1)
                    {
                        // Options
                        window.clear(sf::Color::Black);
                        window.display();
                        optionsMenu(window);
                    }
                }
            }
        }

        //after every change , clear by setting everything black , go through menu and redraw , display
        window.clear(sf::Color::Black);
        for (const auto& item : menuItems) 
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::playMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Games", font, fontSize_);
    menuItems.emplace_back("Tests", font, fontSize_);

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    {  // Games
                        std::cout << "Games selected" << std::endl;
                        std::filesystem::path currentPath = std::filesystem::current_path();
                        std::filesystem::path programsPath = currentPath / "programs";
                        std::cout << programsPath.string() << std::endl;

                        std::vector<std::string> contents = FileSystem::getDirectoryContents(programsPath.string());
                        gameMenu(window, contents);
                    }
                    else if (selectedItemIndex == 1)
                    { // Tests
                        std::cout << "Tests selected" << std::endl;
                        std::filesystem::path currentPath = std::filesystem::current_path();
                        std::filesystem::path testROMPath = currentPath / "testROMs";
                        std::cout << testROMPath.string() << std::endl;

                        std::vector<std::string> contents = FileSystem::getDirectoryContents(testROMPath.string());
                        testMenu(window, contents);
                        // Show tests menu
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    mainMenu(window);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::optionsMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Game Speed", font, fontSize_);
    menuItems.emplace_back("Color Theme", font, fontSize_);
    menuItems.emplace_back("Window Size", font, fontSize_);


    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }


    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    {
                        std::cout << "Game speed menu chosen \n";
                        gameSpeedMenu(window);
                    }
                    else if (selectedItemIndex == 1)
                    {
                        std::cout << "Color Theme menu chosen \n";
                        colorThemeMenu(window);

                    }
                    else if (selectedItemIndex == 2)
                    {
                        std::cout << "Window size menu chosen \n";
                        windowSizeMenu(window);
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    mainMenu(window);
                }
            }
        }


        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::gameSpeedMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Slow", font, fontSize_);
    menuItems.emplace_back("Normal", font, fontSize_);
    menuItems.emplace_back("Fast", font, fontSize_);

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    {
                        std::cout << "Game speed set to slow \n";
                        gameSpeed_ = 2000;
                    }
                    else if (selectedItemIndex == 1)
                    {
                        std::cout << "Game speed set to normal \n";
                        gameSpeed_ = 1500;
                    }
                    else if (selectedItemIndex == 2)
                    {
                        std::cout << "Game speed set to fast \n";
                        gameSpeed_ = 900;
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    optionsMenu(window);
                }
            }
        }


        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}



void ChipApp::gameMenu(sf::RenderWindow& window, const std::vector<std::string>& programContents)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;


    for (const auto& game : programContents)
    {
        menuItems.emplace_back(game, font, fontSize_);
    }

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Selected game: " << programContents[selectedItemIndex] << std::endl;
                    // Launch the chip Manager with selected game
                    std::string gamePath = "./programs/" + programContents[selectedItemIndex];
                    std::cout << "Selected game path: " << gamePath << std::endl;

                    window.close();
                    Chip chip;
                    chip.init();
                    chip.loadProgram(gamePath);

                    ChipManager chipManager(chip);
                    soundPath_ = "./sounds/1ms_beep_sequence.wav";
                    chipManager.setParameters(scaleFactor_, gameSpeed_, backgroundColor_, foregroundColor_, soundPath_);
                    chipManager.start();
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    playMenu(window);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::testMenu(sf::RenderWindow& window, const std::vector<std::string>& programContents)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;


    for (const auto& game : programContents)
    {
        menuItems.emplace_back(game, font, fontSize_);
    }

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Selected game: " << programContents[selectedItemIndex] << std::endl;
                    // Launch the chip Manager with selected game
                    std::string gamePath = "./testROMs/" + programContents[selectedItemIndex];
                    std::cout << "Selected game path: " << gamePath << std::endl;

                    window.close();
                    Chip chip;
                    chip.init();
                    chip.loadProgram(gamePath);

                    ChipManager chipManager(chip);
                    soundPath_ = "./sounds/1ms_beep_sequence.wav";
                    chipManager.setParameters(scaleFactor_, gameSpeed_, backgroundColor_, foregroundColor_, soundPath_);
                    chipManager.start();
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    playMenu(window);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::colorThemeMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Color 1", font, fontSize_);
    menuItems.emplace_back("Color 2", font, fontSize_);

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    {
                        std::cout << "Color 1:  \n";
                        color1Menu(window);
                    }
                    else if (selectedItemIndex == 1)
                    {
                        std::cout << "Color 2: \n";
                        color2Menu(window);
                    }
                    
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    optionsMenu(window);
                }
            }
        }


        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::windowSizeMenu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<sf::Text> menuItems;
    menuItems.emplace_back("Normal", font, fontSize_);
    menuItems.emplace_back("Big", font, fontSize_);
    menuItems.emplace_back("Enourmous", font, fontSize_);

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height;
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if (selectedItemIndex == 0)
                    {
                        std::cout << "Window size - normal. scaleFactor_ = 1  \n";
                        scaleFactor_ = 1;
                    }
                    else if (selectedItemIndex == 1)
                    {
                        std::cout << "Window size - big. scaleFactor_ = 2  \n";
                        scaleFactor_ = 2;
                    }
                    else if (selectedItemIndex == 2)
                    {
                        std::cout << "Window size - enourmous. scaleFactor_ = 3  \n";
                        scaleFactor_ = 3;
                    }

                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    optionsMenu(window);
                }
            }
        }


        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}

void ChipApp::color1Menu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<std::pair<std::string, sf::Color>> colors = {
        {"Black", sf::Color::Black},
        {"White", sf::Color::White},
        {"Red", sf::Color::Red},
        {"Green", sf::Color::Green},
        {"Blue", sf::Color::Blue},
        {"Yellow", sf::Color::Yellow},
        {"Magenta", sf::Color::Magenta},
        {"Cyan", sf::Color::Cyan},
        {"Transparent", sf::Color::Transparent}
    };

    std::vector<sf::Text> menuItems;
    for (const auto& color : colors)
    {
        sf::Text text(color.first, font, fontSize_);
        menuItems.push_back(text);
    }

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height + 20; // Adding spacing
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Selected color: " << colors[selectedItemIndex].first << "\n";
                    backgroundColor_ = colors[selectedItemIndex].second;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    colorThemeMenu(window);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}


void ChipApp::color2Menu(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("munro.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return;
    }

    std::vector<std::pair<std::string, sf::Color>> colors = {
        {"Black", sf::Color::Black},
        {"White", sf::Color::White},
        {"Red", sf::Color::Red},
        {"Green", sf::Color::Green},
        {"Blue", sf::Color::Blue},
        {"Yellow", sf::Color::Yellow},
        {"Magenta", sf::Color::Magenta},
        {"Cyan", sf::Color::Cyan},
        {"Transparent", sf::Color::Transparent}
    };

    std::vector<sf::Text> menuItems;
    for (const auto& color : colors)
    {
        sf::Text text(color.first, font, fontSize_);
        menuItems.push_back(text);
    }

    float totalHeight = 0;
    for (const auto& item : menuItems)
    {
        totalHeight += item.getLocalBounds().height + 20; // Adding spacing
    }

    float startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text& menuItem = menuItems[i];
        menuItem.setPosition((window.getSize().x - menuItem.getLocalBounds().width) / 2, startY);
        startY += menuItem.getLocalBounds().height + 20;
    }

    int selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);

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
                    if (selectedItemIndex > 0) {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex--;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedItemIndex < menuItems.size() - 1)
                    {
                        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                        selectedItemIndex++;
                        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Selected color: " << colors[selectedItemIndex].first << "\n";
                    foregroundColor_ = colors[selectedItemIndex].second;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    std::cout << "Going to previous menu \n";
                    colorThemeMenu(window);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& item : menuItems)
        {
            window.draw(item);
        }
        window.display();
    }
}