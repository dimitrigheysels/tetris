#include "player.h"
#include "field.h"
#include "ui.h"
#include "game.h"

Player::Player() : score_(0)
{
    field_ = std::make_shared<Field>();
    field_->add_new_block();
}

GameState Player::down_block()
{
    return field_->down_block();
}

void Player::left_block()
{
    field_->left_block();
}

void Player::right_block()
{
    field_->right_block();
}

void Player::up_block()
{
    field_->up_block();
}

void Player::game_over()
{
}

void Player::display(const UI &ui) const
{
    field_->display(ui);
    ui.render_scoreboard(score_);
}

void Player::update_score(int nr_of_full_lines)
{
    score_ += nr_of_full_lines;
}