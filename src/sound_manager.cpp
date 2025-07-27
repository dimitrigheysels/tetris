

#include "sound_manager.h"

void SoundManager::play()
{
    s = std::make_shared<sf::Sound>(bell);
    s->play();
}