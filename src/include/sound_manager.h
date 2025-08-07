#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
private:
    sf::SoundBuffer bell_buffer = sf::SoundBuffer("../sound/bell.wav");
    sf::SoundBuffer destroy_1_line_buffer = sf::SoundBuffer("../sound/destroy_1_line.mp3");
    sf::SoundBuffer destroy_2_lines_buffer = sf::SoundBuffer("../sound/destroy_2_lines.mp3");
    sf::SoundBuffer destroy_3_lines_buffer = sf::SoundBuffer("../sound/destroy_3_lines.mp3");
    sf::SoundBuffer destroy_4_lines_buffer = sf::SoundBuffer("../sound/destroy_4_lines.mp3");
    std::shared_ptr<sf::Sound> s;

public:
    SoundManager()
    {
    }

    void play_bell();
    void play_destroy_1_line();
    void play_destroy_2_lines();
    void play_destroy_3_lines();
    void play_destroy_4_lines();
};