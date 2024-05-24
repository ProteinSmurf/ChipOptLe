#include <thread>
#include "ChipManager.hpp"
#include "Audio.h"

ChipManager::ChipManager(Chip& chip) : chip(chip) 
{
   
}

void ChipManager::start()
{
    sf::RenderWindow window(sf::VideoMode(scaleFactor_ * windowWidth_, scaleFactor_ * windowHeight_), "ChipOptLe");
    run(window);
}

void ChipManager::setParameters(int scaleFactor, 
    int gameSpeed,
    const sf::Color& backgroundColor, 
    const sf::Color& foregroundColor,
    const std::string& soundPath) 
{
    scaleFactor_ = scaleFactor;
    gameSpeed_ = gameSpeed;
    backgroundColor_ = backgroundColor;
    foregroundColor_ = foregroundColor;
    soundPath_ = soundPath;

    chipDisplayWidth_ = 64;
    chipDisplayHeight_ = 32;

    windowWidth_ = 10 * chipDisplayWidth_;
    windowHeight_ = 10 * chipDisplayHeight_;

}

void ChipManager::run(sf::RenderWindow& window)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window Closed" << std::endl;
                window.close();
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
        if (chip.needsPlaySound())
        {
            //1ms_beep_sequence
            //beep
            Audio::playSound("./sounds/1ms_beep_sequence.wav");
            chip.removePlaySoundFlag();
        }
        
        
        if (chip.needsRedraw())
        {
            window.clear();

            //draw pixel onto screen (represented by rectangle of upscaled size)
            const uint8_t* display = chip.getDisplay();

            int rectangleSize = scaleFactor_ * 10;

            for (int i = 0; i < chipDisplayWidth_ * chipDisplayHeight_; i++)
            {

                sf::RectangleShape pixel;
                pixel.setSize(sf::Vector2f(rectangleSize, rectangleSize));
  
                if (display[i] == 0)
                {
                    pixel.setFillColor(backgroundColor_);
                }
                else
                {
                    pixel.setFillColor(foregroundColor_);
                }

                //position based on chip 64x32 display
                //int x = i % chipDisplayWidth_;
                //int y = i / chipDisplayWidth_;
                // set position based on original position * upscale
                //pixel.setPosition(x * rectangleSize, y * rectangleSize);

                pixel.setOutlineColor(sf::Color::Black);
                pixel.setOutlineThickness(1.0f);

                int x = (i % chipDisplayWidth_) * rectangleSize;
                int y = (i / chipDisplayWidth_) * rectangleSize;
                pixel.setPosition(x, y);
        
                window.draw(pixel);

                 //   (0, 0)    (10, 0)   (20, 0)   (30, 0)  ... (630, 0)
                 //   + -------- + -------- + -------- + -------- + ---- ...
                 //   | 0        | 1        | 2        | 3        |
                 //   +--------  + -------- + -------- + -------- + ---- ...
                 //   | 64       | 65       | 66       | 67       |
                 //   +--------  + -------- + -------- + -------- + ---- ...
                 //   |          |          |          |          |
                 //   |          |          |          |          |
                 //   +--------  + -------- + -------- + -------- + ---- ...

            }
            window.display();

            chip.removeDrawFlag();
        }
        

        //DEFAULT CHIP SPEED: 60 Hz ( according to spec ) 
        // 60 Hz = 60 cycles / second -> cycleTime = 1/60 ~= 0.167 ms
        //std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
        //std::this_thread::sleep_for(std::chrono::nanoseconds(700));

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
    case sf::Keyboard::Num4: return 0xC;  //12
    case sf::Keyboard::Q:    return 0x4;
    case sf::Keyboard::W:    return 0x5;
    case sf::Keyboard::E:    return 0x6;
    case sf::Keyboard::R:    return 0xD;  //13
    case sf::Keyboard::A:    return 0x7;
    case sf::Keyboard::S:    return 0x8;
    case sf::Keyboard::D:    return 0x9;
    case sf::Keyboard::F:    return 0xE;  //14
    case sf::Keyboard::Z:    return 0xA;  //10
    case sf::Keyboard::X:    return 0x0;
    case sf::Keyboard::C:    return 0xB;  //11
    case sf::Keyboard::V:    return 0xF;  //15
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







