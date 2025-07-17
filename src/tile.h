#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

class Block;

class Tile
{
private:
    int x_;
    int y_;

    std::shared_ptr<Block> block_;

    bool is_boundary_{false};
    bool is_fixed_{false};

public:
    Tile(int x, int y) : x_(x), y_(y) {}

    virtual ~Tile() = default;

    void clear()
    {
        is_fixed_ = false;
        block_ = nullptr;
    }

    void remove_block()
    {
        if (block_)
        {
            block_ = nullptr;
        }
    }

    std::shared_ptr<Block> get_block() const
    {
        return block_;
    }

    void set_block(std::shared_ptr<Block> block)
    {
        block_ = block;
    }

    void set_boundary(bool o) { is_boundary_ = o; }

    inline bool is_free() const
    {
        return !is_boundary_ && !block_ && !is_fixed_;
    }

    void set_fixed();

    bool is_boundary() const { return is_boundary_; }
    bool is_fixed() const { return is_fixed_; }

    void update(const std::shared_ptr<Block> block);
    void display(sf::RenderWindow &w);
};
