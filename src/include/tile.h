#pragma once

#include <memory>
#include <optional>

class Block;

class Tile
{
private:
    int r_;
    int c_;

    bool is_part_of_playfield_{false};
    bool is_boundary_{false};
    bool is_fixed_{false};

    std::shared_ptr<Block> block_;
    std::shared_ptr<Block> backup_block_;

public:
    Tile(int r, int c) : r_(r), c_(c), is_part_of_playfield_{false} {}
    virtual ~Tile() = default;

    inline bool is_part_of_playfield() { return is_part_of_playfield_; }
    inline void set_part_of_playfield() { is_part_of_playfield_ = true; }

    void clear();
    std::shared_ptr<Block> get_block() const;
    void set_block(std::shared_ptr<Block> block);
    void unset_block();

    bool is_boundary() const;
    void set_boundary(bool o);

    bool is_fixed() const;
    void set_fixed(const std::shared_ptr<Block> block);

    bool is_free() const;

    void update(const std::shared_ptr<Block> block);

    void toggle();
};
