
#include <experimental/random>
#include <iostream>

#include "field.h"
#include "ui.h"

Field::Field() : top_row_(PLAYFIELD_BOTTOM_ROW)
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            tiles_[row][col] = std::make_shared<Tile>(row, col);
        }
    }

    // determine boundary tiles
    for (int row = 0; row < ROWS; row++)
    {
        tiles_[row][0]->set_boundary(true);
        tiles_[row][COLS - 1]->set_boundary(true);
    }

    for (int col = 0; col < COLS; col++)
    {
        tiles_[ROWS - 1][col]->set_boundary(true);
    }
}

int Field::get_top_row() const
{
    return top_row_;
}

void Field::add_new_block()
{
    if (next_block_)
    {
        current_block_ = next_block_;
    }
    else
    {
        current_block_ = generate_block();
    }

    next_block_ = generate_block();

    update_tiles();
}

void Field::update_tiles()
{
    auto block_row = current_block_->get_position_row();
    auto block_col = current_block_->get_position_col();

    // clear the old 4 tiles above
    if (block_row >= PLAYFIELD_TOP_ROW)
    {
        for (int col = block_col; col < std::min(PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH, block_col + BLOCK_LAYOUT_SIZE); col++)
        {
            if (!tiles_[block_row - 1][col]->is_fixed())
            {
                tiles_[block_row - 1][col]->unset_block();
            }
        }
    }

    // clear old 4 tiles left
    if (block_col > PLAYFIELD_FIRST_COL) // do not clear boundary of field
    {
        for (int row = block_row; row < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, block_row + BLOCK_LAYOUT_SIZE); row++)
        {
            if (!tiles_[row][block_col - 1]->is_fixed())
            {
                tiles_[row][block_col - 1]->unset_block();
            }
        }
    }

    // clear old 4 tiles right
    if (block_col < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH - BLOCK_LAYOUT_SIZE) // do not clear boundary of field
    {
        for (int row = block_row; row < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, block_row + BLOCK_LAYOUT_SIZE); row++)
        {
            if (!tiles_[row][block_col + BLOCK_LAYOUT_SIZE]->is_fixed())
            {
                tiles_[row][block_col + BLOCK_LAYOUT_SIZE]->unset_block();
            }
        }
    }

    for (int row = block_row; row < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, block_row + BLOCK_LAYOUT_SIZE); row++)
    {
        for (int col = block_col; col < std::min(PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH, block_col + BLOCK_LAYOUT_SIZE); col++)
        {
            if (!tiles_[row][col]->is_fixed())
            {
                tiles_[row][col]->update(current_block_);
            }
        }
    }
}

GameState Field::down_block()
{
    GameState state{};

    if (current_block_->can_down(tiles_))
    {
        current_block_->down();
    }
    else
    {
        // check for gameover
        int r = current_block_->get_position_row();
        if (r == PLAYFIELD_TOP_ROW)
        {
            state.game_over = true;
            return state;
        }

        current_block_->set_fixed_in_field(tiles_);

        // process full lines...
        int nr_of_full_lines = 0;
        int full_line_indexes[BLOCK_LAYOUT_SIZE] = {0};
        nr_of_full_lines = check_full_lines(r, full_line_indexes);

        // ...  and move tiles down
        for (int row_index : full_line_indexes)
        {
            if (row_index != 0)
            {
                // start on row with full line
                // stop on row 1 (row 0 is hidden)
                for (int row = row_index; row >= PLAYFIELD_TOP_ROW; row--)
                {
                    for (int col = PLAYFIELD_FIRST_COL; col < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; col++)
                    {
                        auto b = tiles_[row - 1][col]->get_block();
                        if (b)
                        {
                            tiles_[row][col]->set_fixed(b);
                        }
                        else
                        {
                            tiles_[row][col]->clear();
                        }
                    }
                }
            }
        }

        top_row_ = std::min(top_row_, current_block_->get_top_boundary());
        top_row_ += nr_of_full_lines;

        // show next block
        add_new_block();

        state.new_block = true;
        state.nr_of_full_lines = nr_of_full_lines;
    }

    update_tiles();

    return state;
}

void Field::left_block()
{
    if (current_block_->can_left(tiles_))
    {
        current_block_->left();
    }

    update_tiles();
}

void Field::right_block()
{
    if (current_block_->can_right(tiles_))
    {
        current_block_->right();
    }

    update_tiles();
}

void Field::up_block()
{
    if (current_block_->can_rotate(tiles_))
    {
        current_block_->rotate();
    }

    update_tiles();
}

void Field::clear_lines(int nr_of_lines)
{
    // if field height > 5, clear 5 rows from top of field
    if (top_row_ <= PLAYFIELD_BOTTOM_ROW - nr_of_lines)
    {
        for (int r = top_row_; r < top_row_ + nr_of_lines; r++)
        {
            for (int c = PLAYFIELD_FIRST_COL; c < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; c++)
            {
                tiles_[r][c]->clear();
            }
        }

        top_row_ += nr_of_lines;
    }
}

void Field::scatter_rows(int from_row, int to_row)
{
    std::cout << "scatter_rows - from_row: " << from_row << " to_row: " << to_row << std::endl;
    for (int r = from_row; r < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, to_row); r++)
    {
        for (int c = PLAYFIELD_FIRST_COL; c < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; c++)
        {
            bool should_toggle = std::experimental::randint(0, 1);
            if (should_toggle)
            {
                tiles_[r][c]->toggle();
            }
        }
    }
}

void Field::add_scattered_rows(int from_row, int to_row)
{
    std::cout << "add_scattered_rows - from_row: " << from_row << " to_row: " << to_row << std::endl;
    auto dot_block = std::make_shared<DOT_Block>();
    for (int r = from_row; r > std::max(PLAYFIELD_TOP_ROW, to_row); r--)
    {
        for (int c = PLAYFIELD_FIRST_COL; c < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; c++)
        {
            bool should_add_tile = std::experimental::randint(0, 1);
            if (should_add_tile)
            {
                tiles_[r][c]->set_fixed(dot_block);
            }
        }
    }

    top_row_ += (from_row - to_row);
}

void Field::display(const UI &ui) const
{
    ui.render_tiles(tiles_, top_row_);
    ui.render_next(*next_block_);
}

std::shared_ptr<Block> Field::generate_block() const
{
    auto r = std::experimental::randint(0, 6);
    switch (r)
    {
    case 0:
        return std::make_shared<I_Block>();

    case 1:
        return std::make_shared<S_Block>();

    case 2:
        return std::make_shared<Z_Block>();

    case 3:
        return std::make_shared<T_Block>();

    case 4:
        return std::make_shared<L_Block>();

    case 5:
        return std::make_shared<J_Block>();

    case 6:
        return std::make_shared<O_Block>();
    }

    // should not happen
    return nullptr;
}

int Field::check_full_lines(int from_row, int *full_line_indexes) const
{
    int nr_of_full_lines = 0;
    for (int row = from_row; row < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, from_row + BLOCK_LAYOUT_SIZE); row++)
    {
        if (check_full_line(row))
        {
            full_line_indexes[nr_of_full_lines++] = row;
        }
    }

    return nr_of_full_lines;
}

bool Field::check_full_line(int row) const
{
    for (int col = 1; col < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; col++)
    {
        if (!tiles_[row][col]->is_fixed())
        {
            return false;
        }
    }

    return true;
}