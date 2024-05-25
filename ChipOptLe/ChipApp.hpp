#ifndef CHIPAPP_H
#define CHIPAPP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "FileSystem.hpp"

class ChipApp
{
public:
    ChipApp();
    

    void initialize();
    void run();

private:
    int fontSize_;
    int scaleFactor_;
    int gameSpeed_;
    sf::Color backgroundColor_;
    sf::Color foregroundColor_;
    std::string soundPath_;
    bool soundOn_;
    FileSystem fileSystem_;

    // menus
    void mainMenu(sf::RenderWindow& window);
    void optionsMenu(sf::RenderWindow& window);
    void gameSpeedMenu(sf::RenderWindow& window);
    void colorThemeMenu(sf::RenderWindow& window);
    void color1Menu(sf::RenderWindow& window);
    void color2Menu(sf::RenderWindow& window);
    void customColorMenu(sf::RenderWindow& window);
    void windowSizeMenu(sf::RenderWindow& window);
    void playMenu(sf::RenderWindow& window);
    void soundToggleMenu(sf::RenderWindow& window);
    void chooseSoundFileMenu(sf::RenderWindow& window, const std::vector<std::string>& programContents);
    void gameMenu(sf::RenderWindow& window, const std::vector<std::string>& programContents);
    void testMenu(sf::RenderWindow& window, const std::vector<std::string>& programContents);
};

#endif // CHIPAPP_H
