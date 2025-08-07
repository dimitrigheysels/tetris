#include <spdlog/spdlog.h>

#include "include/state.h"
#include "include/game.h"

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// S T A T I C    F U N C T I O N S /////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// P U B L I C    F U N C T I O N S /////////////////////////

bool StatePlaying::update(const std::optional<sf::Event> &input_event)
{
    bool invalidate = false;
    if (input_event)
    {
        const auto e = input_event->getIf<sf::Event::KeyPressed>();
        if (e != nullptr)
        {
            switch (e->scancode)
            {
            case sf::Keyboard::Scancode::Escape:
            {
                game_.pause();
                invalidate = true;
                break;
            }
            case sf::Keyboard::Scancode::Space:
            {
                game_.drop_block();
                invalidate = true;
                break;
            }
            case sf::Keyboard::Scancode::Down:
            {
                game_.down_block();
                invalidate = true;
                break;
            }
            case sf::Keyboard::Scancode::Left:
            {
                game_.left_block();
                invalidate = true;
                break;
            }
            case sf::Keyboard::Scancode::Right:
            {
                game_.right_block();
                invalidate = true;
                break;
            }
            case sf::Keyboard::Scancode::Up:
            {
                game_.up_block();
                invalidate = true;
                break;
            }
            }
        }
    }
    else
    {
        game_.progress_game();
        invalidate = true;
    }

    game_.check_level_clock();

    return invalidate;
}

void StatePlaying::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    game_.display_playing_state(window);
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////// P R I V A T E     F U N C T I O N S ////////////////////////

StatePlaying::StatePlaying(Game &game) : State(game)
{
}