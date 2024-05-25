#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Chip.hpp"
#include "AudioModule.hpp"

class ChipManager
{
public:
    ChipManager(Chip& chip);
    void start();
    void setParameters(
        const int scaleFactor,
        const int gameSpeed,
        const sf::Color& backgroundColor,
        const sf::Color& foregroundColor,
        const std::string& soundPath,
        const bool soundOn);


    
    const int* getKeyBuffer() const;
    void printKeyBuffer() const;

private:
    Chip& chip;
    int keyBuffer[16];
    AudioModule audioModule;

    int windowWidth_;
    int windowHeight_;
    int chipDisplayWidth_;
    int chipDisplayHeight_;
    int scaleFactor_;
    int gameSpeed_;
    bool soundOn_;
    std::string soundPath_;
    sf::Color backgroundColor_;
    sf::Color foregroundColor_;

    void run(sf::RenderWindow& window);
    int  mapSFKeyToChip8(sf::Keyboard::Key sfKey);
    void initialize();

};
