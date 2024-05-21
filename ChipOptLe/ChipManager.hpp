#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Chip.hpp"

class ChipManager
{
public:
    ChipManager(Chip& chip, int scale_factor);
    void run(sf::RenderWindow& target);
    //void draw();
    //void draw(sf::RenderWindow& target);

    int  mapSFKeyToChip8(sf::Keyboard::Key sfKey);
    const int* getKeyBuffer() const;
    void printKeyBuffer() const;

private:
    Chip& chip;
    int keyBuffer[16];
    std::unordered_map<int, int> keyIdToKey;

    int window_width;
    int window_height;
    int chip_display_width;
    int chip_display_height;
    int scale_factor;

};
