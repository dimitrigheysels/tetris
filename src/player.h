#pragma once

#include <memory>

#include "global.h"

class Field;
class UI;

class Player : public std::enable_shared_from_this<Player>
{
private:
    std::shared_ptr<Field> field_;

    int score_;

public:
    Player();

    GameState down_block();
    void left_block();
    void right_block();
    void up_block();

    void game_over();
    void update_score(int nr_of_full_lines);

    void render(const UI &ui) const;
};