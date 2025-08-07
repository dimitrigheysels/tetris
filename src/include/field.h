#pragma once

#include <memory>
#include <set>

#include "block.h"
#include "tile.h"
#include "global.h"
#include "level.h"

// structure of file:
// width:height
// for each row, x,y coordinates of the boundary tiles
// row start at 1, col start at 0
// 0,1 11,1
// 0,2 5,2 11,2
class FieldDescription
{
private:
    int width_{0};
    int height_{0};
    int start_col_{0};
    std::vector<std::pair<int, int>> boundary_coordinates_;

public:
    FieldDescription(const std::filesystem::path &path);

    inline int get_width() const { return width_; }
    inline int get_height() const { return height_; }
    inline int get_start_col() const { return start_col_; }

    bool is_valid() const;

    bool contains_boundary_coordinates(int x, int y) const;
};

class UI;

class Field
{
private:
    std::shared_ptr<Tile> tiles_[MAX_ROWS + 1 /*hidden row*/][MAX_COLS];

    std::shared_ptr<Block> next_block_;
    std::shared_ptr<Block> current_block_;

    int start_col_;
    int top_row_;

    // bool read_field_file(const std::filesystem::path &path) const;
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