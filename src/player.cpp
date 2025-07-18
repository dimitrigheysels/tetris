#include "player.h"
#include "field.h"
#include "ui.h"
#include "game.h"

Player::Player() : highscore_(0)
{
    field_ = std::make_shared<Field>();
    field_->add_new_block();
}

void Player::start_new_game()
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
    ui.render_highscore(highscore_);
}

void Player::update_highscore(int score)
{
    if (score > highscore_)
    {
        highscore_ = score;
    }
}