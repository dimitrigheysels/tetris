#include <spdlog/spdlog.h>

#include "state.h"
#include "game.h"

void StatePlaying::init(Game &game)
{
    if (StatePlaying::instance_ == nullptr)
    {
        StatePlaying::instance_ = std::shared_ptr<StatePlaying>(new StatePlaying(game));
    }
}

std::shared_ptr<StatePlaying> &StatePlaying::get_instance()
{
    if (StatePlaying::instance_ == nullptr)
    {
        spdlog::error("StatePlaying state is not initialized");
    }

    return StatePlaying::instance_;
}

StatePlaying::StatePlaying(Game &game) : State(game)
{
}

std::string_view StatePlaying::get_name() const
{
    return "PLAYING";
}

void StatePlaying::update(Game &game, std::optional<sf::Event> input_event)
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
                game_.stop();
                break;
            }
            case sf::Keyboard::Scancode::P:
            {
                game.pause();
                break;
            }
            case sf::Keyboard::Scancode::Space:
            {
                game_.drop_block();
                break;
            }
            case sf::Keyboard::Scancode::Down:
            {
                game_.down_block();
                break;
            }
            case sf::Keyboard::Scancode::Left:
            {
                game_.left_block();
                break;
            }
            case sf::Keyboard::Scancode::Right:
            {
                game_.right_block();
                break;
            }
            case sf::Keyboard::Scancode::Up:
            {
                game_.up_block();
                break;
            }
            }
        }
    }
    else
    {
        game_.progress_game();
    }

    game_.check_level_clock();
}

void StatePlaying::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    game_.display_playing_state(window);
}