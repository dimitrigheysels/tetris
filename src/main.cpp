
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game.h"
#include "sound_manager.h"

int main()
{
    // UI ui;
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode({1280, 1024}), "MyTetris");
    Game game;

    // game loop
    while (game.is_running())
    {

        auto event = window->pollEvent();
        game.update(event);

        window->clear();
        game.display(window);
        window->display();
    }

    // cleanup
    window->close();

    return 0;
}