#include <spdlog/spdlog.h>

#include "menu.h"
#include "resource_manager.hpp"

Menu::Menu(const std::string &title) : title_(title), selected_sequence_nr_(1)
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
        selected_sequence_nr_++;
    }
}

void Menu::goto_previous_menuitem()
{
    if (selected_sequence_nr_ > 0)
    {
        selected_sequence_nr_--;
    }
}

void Menu::select_menuitem()
{
    items_.at(selected_sequence_nr_)->execute();
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
    // spdlog::info("{}", title_);
    // for (auto item : items_)
    // {
    //     item->display(selected_sequence_nr_, window);
    // }
}

MenuItem::MenuItem(int sequence_nr, const std::string &title, std::function<void()> action) : sequence_nr_(sequence_nr), title_(title), action_(action)
{
}

void MenuItem::execute()
{
    action_();
}

void MenuItem::display(int selected_sequence_nr, const std::shared_ptr<sf::RenderWindow> window)
{
    float start_y = 400 + (50 * sequence_nr_);
    auto menuitem_text = sf::Text(*ResourceManager::get_instance()->get_font("default_font"), title_);

    menuitem_text.setPosition({400,
                               start_y});
    menuitem_text.setOutlineThickness(2);

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