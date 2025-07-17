
#include <SFML/Graphics.hpp>
#include "ui.h"
#include "game.h"

int main()
{
    UI ui;
    Game game;
    sf::Clock clock_;

    // game loop
    while (game.is_running())
    {
        sf::Event event;
        /*while*/ if (ui.poll_event(event))
        {
            game.update(event);
        }
        else if (clock_.getElapsedTime().asSeconds() > 0.5f)
        {
            game.update();
            clock_.restart();
        }

        ui.clear();
        game.render(ui);
        ui.display();
    }

    // cleanup
    ui.close();

    return 0;
}