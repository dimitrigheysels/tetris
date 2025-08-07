#include <spdlog/spdlog.h>

#include "include/state.h"
#include "include/menu.h"
#include "include/game.h"

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
    std::shared_ptr<Menu> options_menu = std::make_shared<Menu>("Options");
    std::shared_ptr<MenuItem> start = std::make_shared<MenuItem>(0, "Start", [this]()
                                                                 { game_.start(); });
    std::shared_ptr<MenuItem> options = std::make_shared<MenuItem>(1, "Options", [this, options_menu]()
                                                                   { active_menu_ = options_menu; });
    std::shared_ptr<MenuItem> exit = std::make_shared<MenuItem>(2, "Exit", [this]()
                                                                { game_.exit(); });

    std::shared_ptr<MenuItem> hall_of_fame = std::make_shared<MenuItem>(0, "Hall Of Fame", [this]() {});
    std::shared_ptr<MenuItem> player_profile = std::make_shared<MenuItem>(1, "Player profile", [this]() {});
    std::shared_ptr<MenuItem> back_to_main_menu = std::make_shared<MenuItem>(2, "Back", [this, main_menu]()
                                                                             { active_menu_ = main_menu; });

    main_menu->add_menuitem(start);
    main_menu->add_menuitem(options);
    main_menu->add_menuitem(exit);
    options_menu->add_menuitem(hall_of_fame);
    options_menu->add_menuitem(player_profile);
    options_menu->add_menuitem(back_to_main_menu);

    active_menu_ = main_menu;
}

std::string_view StateMenu::get_name() const
{
    return "MENU";
}

bool StateMenu::update(const std::optional<sf::Event> &input_event)
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
                return true;
            }
            case sf::Keyboard::Scancode::Up:
            {
                active_menu_->goto_previous_menuitem();
                return true;
            }
            case sf::Keyboard::Scancode::Enter:
            {
                active_menu_->select_menuitem();
                return true;
            }
            }
        }
    }

    return false;
}

void StateMenu::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    active_menu_->display(window);
}