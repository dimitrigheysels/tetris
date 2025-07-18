#include "tile.h"
#include "block.h"

void Tile::clear()
{
    is_fixed_ = false;
    if (block_)
    {
        block_ = nullptr;
    }
}

std::shared_ptr<Block> Tile::get_block() const
{
    return block_;
}

void Tile::set_block(std::shared_ptr<Block> block)
{
    block_ = block;
}

void Tile::unset_block()
{
    if (block_)
    {
        block_ = nullptr;
    }
}

bool Tile::is_boundary() const { return is_boundary_; }

void Tile::set_boundary(bool o) { is_boundary_ = o; }

bool Tile::is_fixed() const { return is_fixed_; }

void Tile::set_fixed(const std::shared_ptr<Block> block)
{
    block_ = block;
    is_fixed_ = true;
}

bool Tile::is_free() const
{
    return !is_boundary_ && !block_ && !is_fixed_;
}

void Tile::update(const std::shared_ptr<Block> block)
{
    if (block->get_current_layout()[r_ - block->get_position_row()][c_ - block->get_position_col()])
    {
        set_block(block);
    }
    else
    {
        unset_block();
    }
}
