#include <spdlog/spdlog.h>

#include "menu.h"

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
    spdlog::info("{}", title_);
    for (auto item : items_)
    {
        item->display(selected_sequence_nr_, window);
    }
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
    spdlog::info("{} {}", ((sequence_nr_ == selected_sequence_nr) ? '*' : ' '), title_);
}