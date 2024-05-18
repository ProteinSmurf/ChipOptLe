#include "ChipManager.hpp"
#include <thread>

ChipManager::ChipManager(Chip& chip) : chip(chip)
{
    create(sf::VideoMode(640, 320), "Chip 8 Emulator", sf::Style::Close);
    setFramerateLimit(165);
}



void ChipManager::run()
{
    while (isOpen())
    {


        chip.run();
        if (chip.needsRedraw())
        {
            clear();     // Clear the render target before drawing ( blank screen )
            draw();     //  Draw the pixels onto the render target ( draw pixels based on CipDisplay)
            display(); //   Display the rendered frame
            chip.removeDrawFlag();
        }

        //DEFAULT CHIP SPEED: 60 Hz ( according to spec )
        // 60 Hz = 60 cycles / second -> cycleTime = 1/60 ~= 0.167 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
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
        target.draw(pixel, states); 
    }
}

void ChipManager::draw()
{
    sf::RenderTarget& target = *this; 
    const uint8_t* display = chip.getDisplay();
    for (int i = 0; i < 64 * 32; ++i)
    { 
        // 10x10 rectangle for each pixel to be easier to see
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
        target.draw(pixel); // draw the pixel onto the render target
    }
}
