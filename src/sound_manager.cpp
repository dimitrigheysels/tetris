

#include "include/sound_manager.h"

void SoundManager::play_bell()
{
    s = std::make_shared<sf::Sound>(bell_buffer);
    s->play();
}

void SoundManager::play_destroy_1_line()
{
    s = std::make_shared<sf::Sound>(destroy_1_line_buffer);
    s->play();
}

void SoundManager::play_destroy_2_lines()
{
    s = std::make_shared<sf::Sound>(destroy_2_lines_buffer);
    s->play();
}

void SoundManager::play_destroy_3_lines()
{
    s = std::make_shared<sf::Sound>(destroy_3_lines_buffer);
    s->play();
}

void SoundManager::play_destroy_4_lines()
{
    s = std::make_shared<sf::Sound>(destroy_4_lines_buffer);
    s->play();
}