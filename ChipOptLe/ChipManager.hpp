#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Chip.hpp"

class ChipManager
{
public:
    ChipManager(Chip& chip);
    void run(sf::RenderWindow& window);
    void start();
    void setParameters(
        int scaleFactor,
        int gameSpeed,
        const sf::Color& backgroundColor,
        const sf::Color& foregroundColor,
        const std::string& soundPath);


    int  mapSFKeyToChip8(sf::Keyboard::Key sfKey);
    const int* getKeyBuffer() const;
    void printKeyBuffer() const;

private:
    Chip& chip;
    int keyBuffer[16];
    std::unordered_map<int, int> keyIdToKey;

    int windowWidth_;
    int windowHeight_;
    int chipDisplayWidth_;
    int chipDisplayHeight_;
    int scaleFactor_;
    int gameSpeed_;
    std::string soundPath_;
    sf::Color backgroundColor_;
    sf::Color foregroundColor_;

};
