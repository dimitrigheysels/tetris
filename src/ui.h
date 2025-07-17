#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "field.h"

class UI
{
private:
    std::shared_ptr<sf::RenderWindow> window_;
    sf::Font font;

public:
    UI();
    ~UI() = default;

    void clear();
    void display();

    bool is_open();
    void close();

    // should return a custom structure containing event-data
    bool poll_event(sf::Event &event);

    void display_scoreboard(int score) const;
    void display_field(const Field &field) const;
    void display_next(const Block &block) const;
};