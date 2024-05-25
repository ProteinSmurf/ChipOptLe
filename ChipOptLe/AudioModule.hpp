#ifndef AUDIOMODULE_HPP
#define AUDIOMODULE_HPP

#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

class AudioModule 
{
public:
    AudioModule(); 
    void playSound(const std::string& file);

};

#endif // AUDIOMODULE_HPP
