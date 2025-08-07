#pragma once

#include <memory>
#include <set>

#include "block.h"
#include "tile.h"
#include "global.h"
#include "level.h"
#include "field_description.h"

class Field
{
private:
    int start_col_;
    int top_row_;

    std::shared_ptr<Tile> tiles_[MAX_ROWS + 1 /*hidden row*/][MAX_COLS];
    std::shared_ptr<Block> next_block_;
    std::shared_ptr<Block> current_block_;

    std::shared_ptr<Block> generate_block() const;
    int check_full_lines(int from_row, int *nr_of_full_line) const;
    bool check_full_line(int row) const;

    void render_next_block(const std::shared_ptr<sf::RenderWindow> window) const;
    void render_tiles(const std::shared_ptr<sf::RenderWindow> window) const;

public:
    Field();

    inline int get_top_row() const { return top_row_; }

    void init(const FieldDescription &field_description);
    void reset();

    void add_new_block();
    void update_tiles();

    Evaluation down_block();
    Evaluation drop_block();
    void left_block();
    void right_block();
    void up_block();

    void clear_lines(int nr_of_lines);
    void scatter_rows(int from_row, int to_row);
    void add_scattered_rows(int from_row, int to_row);

    void display(const std::shared_ptr<sf::RenderWindow> window) const;
};