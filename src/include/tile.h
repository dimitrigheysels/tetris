#pragma once

#include <memory>

class Block;

class Tile
{
private:
    int r_;
    int c_;

    bool is_part_of_playfield_;
    bool is_boundary_;
    bool is_fixed_;

    std::shared_ptr<Block> block_;
    std::shared_ptr<Block> backup_block_;

public:
    Tile(int r, int c) : r_(r), c_(c), is_part_of_playfield_{false}, is_boundary_{false}, is_fixed_{false} {}
    virtual ~Tile() = default;

    inline bool is_part_of_playfield() const { return is_part_of_playfield_; }
    inline void set_part_of_playfield() { is_part_of_playfield_ = true; }
    inline bool is_boundary() const { return is_boundary_; };
    inline void set_boundary(bool b) { is_boundary_ = b; };
    inline bool is_fixed() const { return is_fixed_; };
    inline bool is_free() const { return !is_boundary_ && !block_ && !is_fixed_; };
    inline std::shared_ptr<Block> get_block() const { return block_; };
    inline void set_block(std::shared_ptr<Block> block) { block_ = block; };

    void clear();
    void unset_block();
    void set_fixed(const std::shared_ptr<Block> block);
    void toggle();
    void update(const std::shared_ptr<Block> block);
};
