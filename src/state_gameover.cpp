#include <spdlog/spdlog.h>

#include "include/state.h"
#include "include/game.h"

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// S T A T I C    F U N C T I O N S /////////////////////////

void StateGameOver::init(Game &game)
{
    if (StateGameOver::instance_ == nullptr)
    {
        StateGameOver::instance_ = std::shared_ptr<StateGameOver>(new StateGameOver(game));
    }
}

std::shared_ptr<StateGameOver> &StateGameOver::get_instance()
{
    if (StateGameOver::instance_ == nullptr)
    {
        spdlog::error("StateGameOver state is not initialized");
    }

    return StateGameOver::instance_;
}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// P U B L I C    F U N C T I O N S /////////////////////////

bool StateGameOver::update(const std::optional<sf::Event> &input_event)
{
    if (input_event)
    {
        const auto e = input_event->getIf<sf::Event::KeyPressed>();
        if (e != nullptr)
        {
            switch (e->scancode)
            {
            case sf::Keyboard::Scancode::Y:
            {
                game_.start();
                return true;
            }
            case sf::Keyboard::Scancode::N:
            {
                game_.stop();
                return true;
            }
            }
        }
    }

    return false;
}

void StateGameOver::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    game_.display_gameover_state(window);
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////// P R I V A T E     F U N C T I O N S ////////////////////////

StateGameOver::StateGameOver(Game &game) : State(game) {}