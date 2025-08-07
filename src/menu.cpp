
#include "include/menu.h"
#include "include/resource_manager.hpp"

Menu::Menu(const std::string &title) : title_(title), selected_sequence_nr_(0)
{
}

void Menu::add_menuitem(const std::shared_ptr<MenuItem> item)
{
    items_.push_back(item);
}

void Menu::goto_next_menuitem()
{
    if (selected_sequence_nr_ < items_.size() - 1)
    {
        while (!items_.at(++selected_sequence_nr_)->is_enabled())
            ;
    }
}

void Menu::goto_previous_menuitem()
{
    if (selected_sequence_nr_ > 0)
    {
        while (!items_.at(--selected_sequence_nr_)->is_enabled())
            ;
    }
}

void Menu::select_menuitem()
{
    items_.at(selected_sequence_nr_)->execute();
    selected_sequence_nr_ = 0;
}

void Menu::reset_selection()
{
    selected_sequence_nr_ = -10;
    goto_next_menuitem();
}

void Menu::display(const std::shared_ptr<sf::RenderWindow> window)
{
    auto title_image = ResourceManager::get_instance()->get_image("title");
    auto title_texture = sf::Texture(*title_image);
    auto title_sprite = sf::Sprite(title_texture);

    auto image_width = title_image->getSize().x;
    auto image_height = title_image->getSize().y;

    title_sprite.setPosition({(window->getSize().x - image_width) / 2.0f,
                              0});

    for (const auto &item : items_)
    {
        item->display(selected_sequence_nr_, window);
    }

    window->draw(title_sprite);
}

MenuItem::MenuItem(int sequence_nr, const std::string &title, std::function<bool()> is_enabled, std::function<void()> action) : sequence_nr_(sequence_nr), title_(title), is_enabled_(is_enabled), action_(action)
{
}

bool MenuItem::is_enabled()
{
    return is_enabled_();
}

void MenuItem::execute()
{
    if (is_enabled_())
    {
        action_();
    }
}

void MenuItem::display(int selected_sequence_nr, const std::shared_ptr<sf::RenderWindow> window)
{
    float start_y = 400 + (50 * sequence_nr_);
    auto menuitem_text = sf::Text(*ResourceManager::get_instance()->get_font("default_font"), title_);

    menuitem_text.setPosition({400,
                               start_y});
    menuitem_text.setOutlineThickness(2);
    if (is_enabled_())
    {
        menuitem_text.setFillColor(sf::Color::White);
    }
    else
    {
        menuitem_text.setFillColor(sf::Color(128, 128, 128));
    }

    if (sequence_nr_ == selected_sequence_nr)
    {
        auto menuitem_textwidth = menuitem_text.getLocalBounds().size.x;
        auto menuitem_textheight = menuitem_text.getLocalBounds().size.y;

        auto selection_rect = sf::RectangleShape({menuitem_textwidth + 20, menuitem_textheight + 15});
        selection_rect.setPosition({menuitem_text.getPosition().x - 10, menuitem_text.getPosition().y});
        selection_rect.setFillColor(sf::Color::Transparent);
        selection_rect.setOutlineColor(sf::Color::White);
        selection_rect.setOutlineThickness(-1.0f);

        window->draw(selection_rect);
    }

    window->draw(menuitem_text);
}