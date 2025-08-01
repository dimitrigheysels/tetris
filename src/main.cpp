
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game.h"
#include "sound_manager.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode({1280, 1024}), "MyTetris");

    FieldDescription field_description("../res/standard_field.txt");
    // FieldDescription field_description("../res/piramid_field.txt");
    if (!field_description.is_valid())
    {
        spdlog::error("invalid field description");
        exit(1);
    }

    Game game(field_description);

    // game loop
    while (game.is_running())
    {
        auto event = window->pollEvent();
        game.update(event);

        window->clear();
        game.display(window);
        window->display();
    }

    window->close();

    return 0;
}