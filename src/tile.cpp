
#include "include/tile.h"
#include "include/block.h"

void Tile::clear()
{
    is_fixed_ = false;
    unset_block();
}

void Tile::unset_block()
{
    if (block_)
    {
        block_ = nullptr;
    }
}

void Tile::set_fixed(const std::shared_ptr<Block> block)
{
    block_ = block;
    is_fixed_ = true;
}

void Tile::toggle()
{
    if (block_)
    {
        backup_block_ = block_;
        clear();
    }
    else
    {
        if (backup_block_)
        {
            set_fixed(backup_block_);
            backup_block_ = nullptr;
        }
        else
        {
            auto dot_block = std::make_shared<DOT_Block>();
            set_fixed(dot_block);
        }
    }
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
