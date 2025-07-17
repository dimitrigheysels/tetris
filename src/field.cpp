
#include <experimental/random>
#include <sstream>
#include <iomanip>

#include "field.h"
#include "ui.h"
#include "player.h"

Field::Field()
{
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");

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

void Field::add_new_block()
{
    if (next_block_)
    {
        current_block_ = next_block_;
        next_block_ = generate_block();
    }
    else
    {
        current_block_ = generate_block();
        next_block_ = generate_block();
    }

    update_tiles();
}

void Field::update_tiles()
{
    auto block_row = current_block_->get_position_row();
    auto block_col = current_block_->get_position_col();

    // clear the old 4 tiles above
    if (block_row > 0)
    {
        for (int col = block_col; col < std::min(COLS - 1, block_col + 4); col++)
        {
            if (!tiles_[block_row - 1][col]->is_fixed())
            {
                tiles_[block_row - 1][col]->remove_block();
            }
        }
    }

    // clear old 4 tiles left
    if (block_col > 0) // do not clear boundary of field
    {
        for (int row = block_row; row < std::min(ROWS - 1, block_row + 4); row++)
        {
            if (!tiles_[row][block_col - 1]->is_fixed())
            {
                tiles_[row][block_col - 1]->remove_block();
            }
        }
    }

    // clear old 4 tiles right
    if (block_col < COLS - 1 - 4) // do not clear boundary of field
    {
        for (int row = block_row; row < std::min(ROWS - 1, block_row + 4); row++)
        {
            if (!tiles_[row][block_col + 3 + 1]->is_fixed())
            {
                tiles_[row][block_col + 3 + 1]->remove_block();
            }
        }
    }

    for (int row = block_row; row < std::min(ROWS - 1, block_row + 4); row++)
    {
        for (int col = block_col; col < block_col + 4; col++)
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
        int r = current_block_->get_position_row();
        int c = current_block_->get_position_col();

        // check for gameover
        if (r <= 1)
        {
            state.game_over = true;
            return state;
        }

        // consolidate block
        for (int row = r; row < r + 4; row++)
        {
            for (int col = c; col < c + 4; col++)
            {
                if (current_block_->get_current_layout()[row - r][col - c])
                {
                    tiles_[row][col]->set_fixed();
                }
            }
        }

        // process full lines...
        int nr_of_full_lines = 0;
        int full_line_indexes[4] = {0};

        nr_of_full_lines = check_full_lines(r, full_line_indexes);

        // ...  and move tiles down
        for (int row_index : full_line_indexes)
        {
            if (row_index != 0)
            {
                // start on row with full line
                // stop on row 1 (row 0 is hidden)
                for (int row = row_index; row >= 1; row--)
                {
                    for (int col = 1; col < COLS - 1; col++)
                    {
                        auto b = tiles_[row - 1][col]->get_block();
                        if (b)
                        {
                            tiles_[row][col]->set_block(b);
                            tiles_[row][col]->set_fixed();
                        }
                        else
                        {
                            tiles_[row][col]->clear();
                        }
                    }
                }
            }
        }

        add_new_block();

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

void Field::render(const UI &ui) const
{
    ui.display_field(*this);
    ui.display_next(*next_block_);
}

void Field::display(sf::RenderWindow &w) const
{
    for (int row = 1; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            tiles_[row][col]->display(w);
        }
    }

    auto r = sf::RectangleShape();
    r.setSize(sf::Vector2f(810.0, 1000.0));

    r.setOutlineColor(sf::Color::Blue);
    r.setOutlineThickness(1);
    r.move(450, 20);

    std::stringstream ss;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j += 4)
        {
            ss << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j
               << " fixed: " << tiles_[i][j]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 1
               << " fixed: " << tiles_[i][j + 1]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 1]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 2
               << " fixed: " << tiles_[i][j + 2]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 2]->get_block()
               << "\t"
               << std::right << std::setw(2) << i << "@" << std::left << std::setw(2) << j + 3
               << " fixed: " << tiles_[i][j + 3]->is_fixed()
               << " block: " << std::right << std::setw(14) << tiles_[i][j + 3]->get_block() << std::endl;
        }
    }

    // for (const auto &b : all_blocks)
    // {
    //     if (auto x = b.lock())
    //     {
    //         ss << "(" << x->get_position_row() << ", " << x->get_position_col() << "): " << b.use_count() << " references" << std::endl;
    //     }
    // }

    sf::Text t(ss.str(), font, 10);
    t.setPosition(r.getPosition().x + 1, r.getPosition().y + 5);

    t.setOutlineThickness(1);

    w.draw(r);
    w.draw(t);
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
    for (int row = from_row; row < std::min(ROWS - 1, from_row + 4); row++)
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
    for (int col = 1; col < COLS - 1; col++)
    {
        if (!tiles_[row][col]->is_fixed())
        {
            return false;
        }
    }

    return true;
}