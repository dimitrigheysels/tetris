#include <spdlog/spdlog.h>

#include "state.h"
#include "game.h"

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

StateGameOver::StateGameOver(Game &game) : State(game) {}

std::string_view StateGameOver::get_name() const
{
    return "GAME OVER";
}

void StateGameOver::update(std::optional<sf::Event> input_event)
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
                break;
            }
            case sf::Keyboard::Scancode::N:
            {
                game_.stop();
                break;
            }
            }
        }
    }
}

void StateGameOver::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    game_.display_gameover_state(window);
}