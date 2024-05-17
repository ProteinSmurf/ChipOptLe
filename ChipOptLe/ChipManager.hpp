#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Chip.hpp"

class ChipManager : public sf::RenderWindow, public sf::Drawable, public sf::Transformable
{
public:
    ChipManager(Chip& chip); 
    void run();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Chip& chip;
    //int keyBuffer[16];

};
