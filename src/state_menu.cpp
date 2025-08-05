#include <spdlog/spdlog.h>

#include "state.h"
#include "menu.h"
#include "game.h"

void StateMenu::init(Game &game)
{
    if (StateMenu::instance_ == nullptr)
    {
        StateMenu::instance_ = std::shared_ptr<StateMenu>(new StateMenu(game));
    }
}

std::shared_ptr<StateMenu> &StateMenu::get_instance()
{
    if (StateMenu::instance_ == nullptr)
    {
        spdlog::error("StateMenu state is not initialized");
    }

    return StateMenu::instance_;
}

StateMenu::StateMenu(Game &game) : State(game)
{
    std::shared_ptr<Menu> main_menu = std::make_shared<Menu>("Main menu");
    std::shared_ptr<MenuItem> start = std::make_shared<MenuItem>(0, "Start", [this]()
                                                                 { game_.start(); });
    std::shared_ptr<MenuItem> exit = std::make_shared<MenuItem>(1, "Exit", [this]()
                                                                { game_.exit(); });

    main_menu->add_menuitem(start);
    main_menu->add_menuitem(exit);

    active_menu_ = main_menu;
}

std::string_view StateMenu::get_name() const
{
    return "MENU";
}

void StateMenu::update(std::optional<sf::Event> input_event)
{
    if (input_event)
    {
        const auto e = input_event->getIf<sf::Event::KeyPressed>();
        if (e != nullptr)
        {
            switch (e->scancode)
            {
            case sf::Keyboard::Scancode::Down:
            {
                active_menu_->goto_next_menuitem();
                break;
            }
            case sf::Keyboard::Scancode::Up:
            {
                active_menu_->goto_previous_menuitem();
                break;
            }
            case sf::Keyboard::Scancode::Enter:
            {
                active_menu_->select_menuitem();
                break;
            }
            }
        }
    }
}

void StateMenu::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    active_menu_->display(window);
}