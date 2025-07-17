#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "block.h"
#include "tile.h"
#include "global.h"

class UI;
class Player;

class Field
{
private:
    std::shared_ptr<Tile> tiles_[ROWS][COLS];

    std::shared_ptr<Block> next_block_;
    std::shared_ptr<Block> current_block_;

    std::weak_ptr<Player> player_;

    // bool is_started;
    //  std::vector<std::weak_ptr<Block>> all_blocks;
    sf::Font font;

    std::shared_ptr<Block> generate_block() const;

public:
    Field();

    // void start();

    void add_new_block();
    void update_tiles();

    GameState down_block();
    void left_block();
    void right_block();
    void up_block();

    void render(const UI &ui) const;
    void display(sf::RenderWindow &w) const;
};