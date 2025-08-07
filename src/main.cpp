
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>

#include "include/game.h"
// #include "include/sound_manager.h"
#include "include/resource_manager.hpp"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    ResourceManager::init("../res/resources.json");

    std::shared_ptr<sf::RenderWindow>
        window = std::make_shared<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "MyTetris", sf::State::Fullscreen);
    window->setFramerateLimit(30);

    FieldDescription field_description("../res/standard_field.txt");
    // FieldDescription field_description("../res/piramid_field.txt");
    if (!field_description.is_valid())
    {
        spdlog::error("invalid field description");
        exit(1);
    }

    std::shared_ptr<Game> game = std::make_shared<Game>(field_description);
    game->init();

    // game loop
    while (game->is_running())
    {
        auto event = window->pollEvent();

        // only display window when game is updated
        // (otherwise much lag when moving mouse for example)
        if ((event && event->is<sf::Event::Resized>()) ||
            (game->update(event)))
        {
            window->clear();
            game->display(window);
            window->display();
        }
    }
    window->close();

    return 0;
}