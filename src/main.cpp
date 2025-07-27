
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "ui.h"
#include "game.h"
#include "sound_manager.h"

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