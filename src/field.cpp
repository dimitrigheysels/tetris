
#include <spdlog/spdlog.h>
#include <experimental/random>
#include <iostream>
#include <fstream>
#include <ranges>

#include "field.h"

FieldDescription::FieldDescription(const std::filesystem::path &path)
{
    spdlog::info("Loading field description '{}' ...", path.filename().c_str());

    std::ifstream in(path, std::fstream::in);
    long maxsize = std::numeric_limits<std::streamsize>::max();
    if (in.is_open())
    {
        std::string line;
        std::getline(in, line);
        std::istringstream iss(line);
        iss >> height_;
        iss.ignore(maxsize, ':');
        iss >> width_;

        std::getline(in, line);
        std::istringstream iss2(line);
        iss2 >> start_col_;

        while (std::getline(in, line))
        {
            std::istringstream iss(line);
            while (!iss.eof())
            {
                std::pair<int, int> coordinates;
                iss >> coordinates.first;
                iss.ignore(maxsize, ',');
                iss >> coordinates.second;
                iss.ignore(maxsize, ' ');
                boundary_coordinates_.emplace_back(coordinates);
            }
        }
        in.close();
    }
    else
    {
        spdlog::error("Unable to open field description file");
    }
}

bool FieldDescription::is_valid() const
{
    // field should at least be able to contain a block (4x4)
    if (width_ < 6)
    {
        spdlog::info("Field description 'width' is too small");
        return false;
    }
    if (height_ < 4)
    {
        spdlog::info("Field description 'height' is too small");
        return false;
    }
    // field should have at least two boundary tiles for each row
    if (boundary_coordinates_.size() < height_ * 2)
    {
        spdlog::info("Field description number of 'boundary coordinates' is too small");
        return false;
    }
    // start position should be between the boundary coordinates of first row
    if (start_col_ <= boundary_coordinates_.at(0).second || start_col_ >= boundary_coordinates_.at(1).second)
    {
        spdlog::info("Field description 'start position' is invalid");
        return false;
    }

    // x-coordinate must be between 0 and width
    // y-coordinate must be between 1 and height
    for (const auto &[row, col] : boundary_coordinates_)
    {
        if (col < 0 || col >= width_)
        {
            spdlog::info("Field description column '{}' of coordinate '{},{}' is invalid", col, row, col);
            return false;
        }
        if (row < 1 || row > height_)
        {
            spdlog::info("Field description row '{}' of coordinate '{},{}' is invalid", row, row, col);
            return false;
        }
    }

    return true;
}

bool FieldDescription::contains_boundary_coordinates(int row, int col) const
{
    return std::ranges::find_if(boundary_coordinates_, [row, col](const std::pair<int, int> &p)
                                { return p.first == row && p.second == col; }) != boundary_coordinates_.end();
}

Field::Field()
{
    for (int row = 0; row < MAX_ROWS + 1; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            tiles_[row][col] = std::make_shared<Tile>(row, col);
        }
    }
}

Field::Field(const FieldDescription &field_description) : Field()
{
    spdlog::info("Initializing field ...");

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
    for (int row = 0; row < MAX_ROWS; row++)
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

GameState Field::drop_block()
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
    for (int row = from_row; row < std::min(PLAYFIELD_TOP_ROW + PLAYFIELD_HEIGHT, from_row + BLOCK_LAYOUT_SIZE); row++)
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
    for (int col = 1; col < PLAYFIELD_FIRST_COL + PLAYFIELD_WIDTH; col++)
    {
        if (!(tiles_[row][col]->is_fixed() || tiles_[row][col]->is_boundary()))
        {
            return false;
        }
    }

    return true;
}