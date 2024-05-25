#include "AudioModule.hpp"


AudioModule::AudioModule() 
{
   
}

void AudioModule::playSound(const std::string& file)
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

    //Wait until the sound finishes playing
   /*while (sound.getStatus() == sf::Sound::Playing) {}*/
    sf::sleep(sf::milliseconds(100));

    std::cout << "Beep" << std::endl;
}
