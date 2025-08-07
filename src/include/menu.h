
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include <SFML/Graphics.hpp>

class MenuItem
{
private:
    int sequence_nr_;
    std::string title_;
    std::function<void()> action_;

public:
    MenuItem(int sequence_nr, const std::string &title, std::function<void()> action);

    void execute();

    void display(int selected_sequence_nr, const std::shared_ptr<sf::RenderWindow> window);
};

class Menu
{
private:
    std::string title_;
    std::vector<std::shared_ptr<MenuItem>> items_;
    int selected_sequence_nr_;

public:
    Menu(const std::string &title);

    void add_menuitem(const std::shared_ptr<MenuItem> item);
    void goto_next_menuitem();
    void goto_previous_menuitem();
    void select_menuitem();

    void display(const std::shared_ptr<sf::RenderWindow> window);
};
