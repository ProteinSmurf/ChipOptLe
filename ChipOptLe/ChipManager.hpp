#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Chip.hpp"

class ChipManager
{
public:
    ChipManager(Chip& chip, int scaleFactor);
    void run(sf::RenderWindow& window);
    void setDefaultParameters();


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
    sf::Color backgroundColor_;
    sf::Color foregroundColor_;

};
