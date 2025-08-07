
#include <spdlog/spdlog.h>
#include <exception>

#include "include/field.h"

Field::Field()
{
    spdlog::debug("Field::Field()");
    for (int row = 0; row < MAX_ROWS + 1; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            tiles_[row][col] = std::make_shared<Tile>(row, col);
        }
    }
}

void Field::init(const FieldDescription &field_description)
{
    spdlog::info("Initializing field ...");

    if (!field_description.is_valid())
    {
        spdlog::error("Invalid field description");
        throw std::exception();
    }

    // create playfield
    for (int row = 1; row <= field_description.get_height(); row++)
    {
        for (int col = 0; col < field_description.get_width(); col++)
        {
            tiles_[row][col]->set_part_of_playfield();
            if (field_description.contains_boundary_coordinates(row, col))
            {
                tiles_[row][col]->set_boundary(true);
            }
        }
    }

    start_col_ = field_description.get_start_col();
    top_row_ = field_description.get_height();
}

void Field::reset()
{
    for (int row = 0; row < MAX_ROWS + 1; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            if (!tiles_[row][col]->is_boundary())
            {
                tiles_[row][col]->clear();
            }
        }
    }
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

Evaluation Field::down_block()
{
    Evaluation evaluation{};

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
            evaluation.game_over = true;
            return evaluation;
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

        evaluation.new_block = true;
        evaluation.nr_of_full_lines = nr_of_full_lines;
    }

    update_tiles();

    return evaluation;
}

Evaluation Field::drop_block()
{
    while (current_block_->can_down(tiles_))
    {
        current_block_->down();
        update_tiles();
    }

    return down_block();
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
    spdlog::debug("Clear {} lines from row {}", nr_of_lines, top_row_);

    // if field height > 5, clear 5 rows from top of field
    if (top_row_ <= PLAYFIELD_BOTTOM_ROW - nr_of_lines)
    {
        for (int r = top_row_; r < top_row_ + nr_of_lines; r++)
        {
            for (int c = PLAYFIELD_FIRST_COL; c < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; c++)
            {
                if (!tiles_[r][c]->is_boundary())
                {
                    tiles_[r][c]->clear();
                }
            }
        }

        top_row_ += nr_of_lines;
    }
}

void Field::scatter_rows(int from_row, int to_row)
{
    spdlog::debug("Scattering rows - from row {} to row {}", from_row, to_row);

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
    spdlog::debug("Adding scattered rows - from row {} to row {}", from_row, to_row);

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

void Field::display(const std::shared_ptr<sf::RenderWindow> window) const
{
    render_tiles(window);
    render_next_block(window);
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////// P R I V A T E     F U N C T I O N S ////////////////////////

std::shared_ptr<Block> Field::generate_block() const
{
    auto r = std::experimental::randint(0, 6);
    switch (r)
    {
    case 0:
        return std::make_shared<I_Block>(start_col_);

    case 1:
        return std::make_shared<S_Block>(start_col_);

    case 2:
        return std::make_shared<Z_Block>(start_col_);

    case 3:
        return std::make_shared<T_Block>(start_col_);

    case 4:
        return std::make_shared<L_Block>(start_col_);

    case 5:
        return std::make_shared<J_Block>(start_col_);

    case 6:
        return std::make_shared<O_Block>(start_col_);
    }

    // should not happen
    return nullptr;
}

int Field::check_full_lines(int from_row, int *full_line_indexes) const
{
    spdlog::debug("Checking for full lines from row {}", from_row);

    int nr_of_full_lines = 0;
    for (int row = from_row; row < std::min(PLAYFIELD_TOP_ROW + /*PLAYFIELD_HEIGHT*/ 25, from_row + BLOCK_LAYOUT_SIZE); row++)
    {
        if (check_full_line(row))
        {
            spdlog::debug("Full lines found at row {}", row);
            full_line_indexes[nr_of_full_lines++] = row;
        }
    }

    return nr_of_full_lines;
}

bool Field::check_full_line(int row) const
{
    // from first boundary tile in row to last boundary tile in row
    for (int col = 1; col < PLAYFIELD_FIRST_COL + /*PLAYFIELD_WIDTH*/ 10; col++)
    {
        if (!(tiles_[row][col]->is_fixed() || tiles_[row][col]->is_boundary()))
        {
            return false;
        }
    }

    return true;
}

void Field::render_next_block(const std::shared_ptr<sf::RenderWindow> window) const
{
    auto layout = next_block_->get_current_layout();

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (layout[row][col])
            {
                auto shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
                shape.setPosition({X_NEXT_BLOCK + (col * SIZE_TILE), Y_NEXT_BLOCK + (row * SIZE_TILE)});
                shape.setOutlineColor(sf::Color::Black);
                shape.setOutlineThickness(-1.0f);
                shape.setFillColor(next_block_->get_color());

                window->draw(shape);
            }
        }
    }
}

void Field::render_tiles(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (int row = 1; row < MAX_ROWS + 1; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            const auto &t = tiles_[row][col];

            sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
            shape.setPosition({col * SIZE_TILE, row * SIZE_TILE});
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(-1.0f);

            if (!t->is_part_of_playfield())
            {
                shape.setFillColor(sf::Color(16, 16, 16));
            }
            else if (t->is_boundary())
            {
                // if (row == top_row_ && (col == 0 || col == PLAYFIELD_WIDTH - 1))
                // {
                //     shape.setFillColor(sf::Color(32, 32, 32));
                // }
                // else
                // {
                shape.setFillColor(sf::Color(128, 128, 128));
                // }
            }
            else if (auto block = t->get_block())
            {
                shape.setFillColor(block->get_color());
            }
            else
            {
                shape.setFillColor(sf::Color(64, 64, 64));
            }

            window->draw(shape);
        }
    }
}
