#pragma once

#include <memory>

#include "block.h"
#include "tile.h"
#include "global.h"
#include "level.h"

class UI;

class Field
{
private:
    std::shared_ptr<Tile> tiles_[ROWS][COLS];

    std::shared_ptr<Block> next_block_;
    std::shared_ptr<Block> current_block_;

    int top_row_;

    std::shared_ptr<Block> generate_block() const;
    int check_full_lines(int from_row, int *nr_of_full_line) const;
    bool check_full_line(int row) const;

public:
    Field();

    int get_top_row() const;

    void add_new_block();
    void update_tiles();

    GameState down_block();
    void left_block();
    void right_block();
    void up_block();

    void clear_lines(int nr_of_lines);
    void scatter_rows(int from_row, int to_row);
    void add_scattered_rows(int from_row, int to_row);

    void display(const UI &ui) const;
};