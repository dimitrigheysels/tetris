#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "block.h"
#include "tile.h"
#include "global.h"

class UI;

class Field
{
private:
    std::shared_ptr<Tile> tiles_[ROWS][COLS];

    std::shared_ptr<Block> next_block_;
    std::shared_ptr<Block> current_block_;

    std::shared_ptr<Block> generate_block() const;
    int check_full_lines(int from_row, int *nr_of_full_line) const;
    bool check_full_line(int row) const;

public:
    Field();

    void add_new_block();
    void update_tiles();

    GameState down_block();
    void left_block();
    void right_block();
    void up_block();

    void display(const UI &ui) const;
};