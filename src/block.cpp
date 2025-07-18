#include "block.h"
#include "field.h"

void Block::set_fixed_in_field(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS])
{

    for (int row = position_row_; row < position_row_ + 4; row++)
    {
        for (int col = position_col_; col < position_col_ + 4; col++)
        {
            if (get_current_layout()[row - position_row_][col - position_col_])
            {
                tiles_[row][col]->set_fixed(shared_from_this());
            }
        }
    }
}

std::optional<int> Block::get_bottom_boundary(int col) const
{
    for (int i = 3; i >= 0; i--)
    {
        if (get_current_layout()[i][col])
        {
            return i;
        }
    }

    return std::nullopt;
}

/// Returns column-index of left-boundary of block for 'row'
std::optional<int> Block::get_left_boundary(int row) const
{
    for (int i = 0; i < 4; i++)
    {
        if (get_current_layout()[row][i])
        {
            return i;
        }
    }

    return std::nullopt;
}

/// Returns column-index of right-boundary of block for 'row'
std::optional<int> Block::get_right_boundary(int row) const
{
    for (int i = 3; i >= 0; i--)
    {
        if (get_current_layout()[row][i])
        {
            return i;
        }
    }

    return std::nullopt;
}

bool Block::can_down(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const
{
    for (int col = position_col_; col < position_col_ + 4; col++)
    {
        auto bottom_boundary = get_bottom_boundary(col - position_col_);
        if (bottom_boundary && !tiles_[position_row_ + bottom_boundary.value() + 1][col]->is_free())
        {
            return false;
        }
    }

    return true;
}

void Block::down()
{
    position_row_++;
}

bool Block::can_left(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const
{
    for (int row = position_row_; row < position_row_ + 4; row++)
    {
        auto left_boundary = get_left_boundary(row - position_row_);
        if (left_boundary && !tiles_[row][position_col_ + left_boundary.value() - 1]->is_free())
        {
            return false;
        }
    }

    return true;
}

void Block::left()
{
    position_col_--;
}

bool Block::can_right(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const
{
    for (int row = position_row_; row < position_row_ + 4; row++)
    {
        auto right_boundary = get_right_boundary(row - position_row_);
        if (right_boundary && !tiles_[row][position_col_ + right_boundary.value() + 1]->is_free())
        {
            return false;
        }
    }

    return true;
}

void Block::right()
{
    position_col_++;
}

bool Block::can_rotate(const std::shared_ptr<Tile> (&tiles_)[ROWS][COLS]) const
{
    auto rot_layout = get_layout_after_rotation();

    for (int row = position_row_; row < position_row_ + 4; row++)
    {
        for (int col = position_col_; col < position_col_ + 4; col++)
        {
            if (rot_layout[row - position_row_][col - position_col_])
            {
                if ((tiles_[row][col]->is_fixed()) || (tiles_[row][col]->is_boundary()))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void Block::rotate()
{
    layout_index_ = (layout_index_ + 1) % 4;
}

const layout_t &I_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &I_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color I_Block::get_color() const
{
    return sf::Color::Red;
}

const layout_t &S_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &S_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color S_Block::get_color() const
{
    return sf::Color::Cyan;
}

const layout_t &Z_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &Z_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color Z_Block::get_color() const
{
    return sf::Color::Green;
}

const layout_t &T_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &T_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color T_Block::get_color() const
{
    return sf::Color::Blue;
}

const layout_t &L_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &L_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color L_Block::get_color() const
{
    return sf::Color::Yellow;
}

const layout_t &J_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &J_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color J_Block::get_color() const
{
    return sf::Color::Magenta;
}

const layout_t &O_Block::get_current_layout() const
{
    return LAYOUTS[layout_index_];
}

const layout_t &O_Block::get_layout_after_rotation() const
{
    return LAYOUTS[layout_index_ + 1 % 4];
}

sf::Color O_Block::get_color() const
{
    return sf::Color::White;
}