#include "ChipManager.hpp"
#include <thread>

ChipManager::ChipManager(Chip& chip) : chip(chip)
{
    create(sf::VideoMode(640, 320), "Chip 8 Emulator", sf::Style::Close);
    setFramerateLimit(165);
}



void ChipManager::run()
{
    chip.run();
}


void ChipManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const uint8_t* display = chip.getDisplay();
    for (int i = 0; i < 64 * 32; i++) 
    {
        sf::RectangleShape pixel(sf::Vector2f(10.f, 10.f));
        if (display[i] == 0)
        {
            pixel.setFillColor(sf::Color::Black);
        }
        else
        {
            pixel.setFillColor(sf::Color::White);
        }

        int x = i % 64;
        int y = i / 64;

        pixel.setPosition(sf::Vector2f(x * 10.f, y * 10.f));
        target.draw(pixel, states); // Draw the pixel onto the render target
    }
}