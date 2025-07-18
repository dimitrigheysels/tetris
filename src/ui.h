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

    void clear() const;
    void display() const;

    bool is_open();
    void close();

    bool poll_event(sf::Event &event);

    void render_scoreboard(int score) const;
    void render_tiles(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const;
    void render_tile(int r, int c, const std::shared_ptr<Tile> tile) const;
    void render_next(const Block &block) const;
};