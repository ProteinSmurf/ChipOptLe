#include "ChipManager.hpp"
#include <thread>

ChipManager::ChipManager(Chip& chip) : chip(chip)
{
    create(sf::VideoMode(640, 320), "Chip 8 Emulator", sf::Style::Close);
    setFramerateLimit(165); //my monitor refresh rate
}


void ChipManager::run()
{
    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
            //std::cout << "Event Type: " << event.type << std::endl; // Print event type
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window Closed" << std::endl;
                close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                int key_ = mapSFKeyToChip8(event.key.code);
                if (key_ != -1)
                {
                    std::cout << "Key Pressed: " << key_ << std::endl; // Print mapped key
                    keyBuffer[key_] = 1;
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                int key_ = mapSFKeyToChip8(event.key.code);
                if (key_ != -1)
                {
                    std::cout << "Key Released: " << key_ << std::endl; // Print mapped key
                    keyBuffer[key_] = 0;
                }
            }
        }

        // Update chip with current key buffer
        chip.setKeyBuffer(getKeyBuffer());
        chip.run();

        if (chip.needsRedraw())
        {
            clear();
            draw();
            display();
            chip.removeDrawFlag();
        }

        //DEFAULT CHIP SPEED: 60 Hz ( according to spec )
        // 60 Hz = 60 cycles / second -> cycleTime = 1/60 ~= 0.167 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
        
    }
}


// Chip-8 Keys
// 16 key hexadecimal keyboard (keys marked 0-F)
// 
// Chip_8_Keys   ->   Interpreter_Keys
// 1  2  3  C         1  2  3  4
// 4  5  6  D         Q  W  E  R
// 7  8  9  E         A  S  D  F
// A  0  B  F         Z  X  C  V
//
int ChipManager::mapSFKeyToChip8(sf::Keyboard::Key sfKey)
{
    //Take SFML keycode and return Chip equivalent
    switch (sfKey)
    {
        case sf::Keyboard::Num1: return 0x1;
        case sf::Keyboard::Num2: return 0x2;
        case sf::Keyboard::Num3: return 0x3;
        case sf::Keyboard::Num4: return 0xC;
        case sf::Keyboard::Q:    return 0x4;
        case sf::Keyboard::W:    return 0x5;
        case sf::Keyboard::E:    return 0x6;
        case sf::Keyboard::R:    return 0xD;
        case sf::Keyboard::A:    return 0x7;
        case sf::Keyboard::S:    return 0x8;
        case sf::Keyboard::D:    return 0x9;
        case sf::Keyboard::F:    return 0xE;
        case sf::Keyboard::Z:    return 0xA;
        case sf::Keyboard::X:    return 0x0;
        case sf::Keyboard::C:    return 0xB;
        case sf::Keyboard::V:    return 0xF;
        default:                  return -1; // Not a Chip-8 key
    }
}

const int* ChipManager::getKeyBuffer() const
{
    return keyBuffer;
}


//debug purposes
void ChipManager::printKeyBuffer() const
{
    std::cout << "Key Buffer:\n";
    for (int i = 0; i < 16; i++)
    {
        std::cout << keyBuffer[i] << " ";
    }
    std::cout << std::endl;
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


