
#include <SFML/Graphics.hpp>
#include "ui.h"
#include "game.h"
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
        game.display(ui);
        ui.display();
    }

    // cleanup
    ui.close();

    return 0;
}