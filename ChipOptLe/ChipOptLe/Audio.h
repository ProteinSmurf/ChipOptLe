#pragma once
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

class Audio {
public:
    static void playSound(const std::string& file)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(file))
        {
            std::cerr << "Failed to load audio file: " << file << std::endl;
            return;
        }

        sf::Sound sound;
        sound.setBuffer(buffer);
        sound.play();

        // Wait until the sound finishes playing
        //while (sound.getStatus() == sf::Sound::Playing) {}
        sf::sleep(sf::milliseconds(200));

        std::cout << "Beep" << std::endl;
    }
};
