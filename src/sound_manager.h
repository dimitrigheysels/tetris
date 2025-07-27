#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
private:
    sf::SoundBuffer bell = sf::SoundBuffer("../sound/Bell.wav");
    std::shared_ptr<sf::Sound> s;

public:
    SoundManager()
    {
        }

    void play();
};