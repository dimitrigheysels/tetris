#pragma once

#include <memory>

#include "global.h"

class Field;
class UI;

class Player : public std::enable_shared_from_this<Player>
{
private:
    std::shared_ptr<Field> field_;

    int highscore_;

public:
    Player();

    void start_new_game();

    GameState down_block();
    void left_block();
    void right_block();
    void up_block();

    void game_over();
    void update_highscore(int score);

    void display(const UI &ui) const;
};