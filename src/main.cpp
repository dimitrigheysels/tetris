
#include <SFML/Graphics.hpp>

#include "ui.h"
#include "game.h"
#include <iostream>

int main()
{
    UI ui;
    Game game;

    // game loop
    while (game.is_running())
    {

        auto event = ui.poll_event();
        game.update(event);

        ui.clear();
        game.display(ui);
        ui.display();
    }

    // cleanup
    ui.close();

    return 0;
}