#include <spdlog/spdlog.h>

#include "state.h"
#include "game.h"

void StatePaused::init(Game &game)
{
    if (StatePaused::instance_ == nullptr)
    {
        StatePaused::instance_ = std::shared_ptr<StatePaused>(new StatePaused(game));
    }
}

std::shared_ptr<StatePaused> &StatePaused::get_instance()
{
    if (StatePaused::instance_ == nullptr)
    {
        spdlog::error("StatePaused state is not initialized");
    }

    return StatePaused::instance_;
}

StatePaused::StatePaused(Game &game) : StatePlaying(game)
{
}

std::string_view StatePaused::get_name() const
{
    return "PAUSED";
}

void StatePaused::update(Game &game, std::optional<sf::Event> input_event)
{
    if (input_event)
    {
        const auto e = input_event->getIf<sf::Event::KeyPressed>();
        if (e != nullptr)
        {
            switch (e->scancode)
            {
            case sf::Keyboard::Scancode::Escape:
            {
                game_.set_state(StateMenu::get_instance());
                break;
            }
            case sf::Keyboard::Scancode::P:
            {
                game.resume();
                break;
            }
            }
        }
    }
}

void StatePaused::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    StatePlaying::display(window);
    // TODO: display text "P A U S E D"
}
