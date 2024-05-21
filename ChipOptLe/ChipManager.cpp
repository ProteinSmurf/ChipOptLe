#include <thread>
#include "ChipManager.hpp"

ChipManager::ChipManager(Chip& chip, int scale_factor) : chip(chip), scale_factor(scale_factor)
{
    window_width = 640;
    window_height = 320;
    chip_display_width = 64;
    chip_display_height = 32;
    if (scale_factor != 1 && scale_factor != 2 && scale_factor != 3)
    {
        scale_factor = 1; //default size
    }

    sf::RenderWindow window(sf::VideoMode(scale_factor * window_width, scale_factor * window_height), "Chip 8 Emulator");
    window.setFramerateLimit(165); //my monitor refresh rate
    run(window);


}

void ChipManager::run(sf::RenderWindow& target)
{
    while (target.isOpen())
    {
        sf::Event event;
        while (target.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window Closed" << std::endl;
                target.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                int key_ = mapSFKeyToChip8(event.key.code);
                if (key_ != -1)
                {
                    std::cout << "Key Pressed: " << key_ << std::endl;
                    keyBuffer[key_] = 1;
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                int key_ = mapSFKeyToChip8(event.key.code);
                if (key_ != -1)
                {
                    std::cout << "Key Released: " << key_ << std::endl;
                    keyBuffer[key_] = 0;
                }
            }
        }

        // Update chip with current key buffer
        chip.setKeyBuffer(getKeyBuffer());
        chip.run();

        if (chip.needsRedraw())
        {
            target.clear();

            //draw pixel onto screen (represented by rectangle of upscaled size)
            const uint8_t* display = chip.getDisplay();

            int rectangleSize = scale_factor * 10;

            for (int i = 0; i < chip_display_width * chip_display_height; i++)
            {

                sf::RectangleShape pixel;
                pixel.setSize(sf::Vector2f(rectangleSize, rectangleSize));
  
                if (display[i] == 0)
                {
                    pixel.setFillColor(sf::Color::Black);
                }
                else
                {
                    pixel.setFillColor(sf::Color::White);
                }

                int x = i % chip_display_width;
                int y = i / chip_display_width;

                pixel.setPosition(x * rectangleSize, y * rectangleSize);
                target.draw(pixel);


            }
            target.display();
            chip.removeDrawFlag();
        }

        //DEFAULT CHIP SPEED: 60 Hz ( according to spec )
        // 60 Hz = 60 cycles / second -> cycleTime = 1/60 ~= 0.167 ms
        /*std::this_thread::sleep_for(std::chrono::milliseconds(16)); */
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000));

    }
}


// Chip-8 Keys
// 16 key hexadecimal keyboard (keys marked 0-F)
// 
//  Chip_8_Keys   ->   Interpreter_Keys
//  1  2  3  C         1  2  3  4
//  4  5  6  D         Q  W  E  R
//  7  8  9  E         A  S  D  F
//  A  0  B  F         Z  X  C  V
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







