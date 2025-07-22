#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

#include "global.h"
#include "tile.h"
#include "block.h"

class UI
{
private:
    std::shared_ptr<sf::RenderWindow> window_;
    sf::Font font_;

public:
    UI();
    ~UI() = default;

    void clear() const;
    void display() const;

    bool is_open();
    void close();

    std::optional<sf::Event> poll_event() const;

    void render_highscore(int score) const;
    void render_scoreboard(int level, int score, int nr_of_lines) const;
    void render_level_countdown(int level_clock_elapsed_seconds, int level_event_countdown) const;
    void render_tiles(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const;
    void render_tile(int r, int c, const std::shared_ptr<Tile> tile) const;
    void render_next(const Block &block) const;
    void render_gameover() const;
};