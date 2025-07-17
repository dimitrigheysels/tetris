#include "tile.h"
#include "block.h"

void Tile::set_fixed()
{
    is_fixed_ = true;
}

void Tile::update(const std::shared_ptr<Block> block)
{
    if (block->get_current_layout()[y_ - block->get_position_row()][x_ - block->get_position_col()])
    {
        set_block(block);
    }
    else
    {
        remove_block();
    }
}

void Tile::display(sf::RenderWindow &w)
{
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(SIZE_TILE, SIZE_TILE));
    shape.setPosition(x_ * SIZE_TILE, y_ * SIZE_TILE);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(-1.0f);

    if (is_boundary_)
    {
        shape.setFillColor(sf::Color(128, 128, 128));
    }
    else if (block_)
    {
        shape.setFillColor(block_->get_color());
    }
    else
    {
        shape.setFillColor(sf::Color(64, 64, 64));
    }

    w.draw(shape);
}