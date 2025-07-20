#include <iostream>

#include "game.h"
#include "global.h"
#include "ui.h"

Game::Game() : is_running_(true), is_paused_(false), is_game_over_(false), score_(0), nr_of_lines_(0), level_(1)
{
    player_ = std::make_shared<Player>();
}

bool Game::is_running()
{
    return is_running_;
}

void Game::update(std::optional<sf::Event> event)
{
    if (event)
    {
        if (event->is<sf::Event::Closed>())
        {
            is_running_ = false;
        }
        else
        {
            const auto e = event->getIf<sf::Event::KeyPressed>();
            if (e != nullptr)
            {
                if (e->scancode == sf::Keyboard::Scancode::Escape)
                {
                    is_running_ = false;
                }
                else if (!is_game_over_)
                {
                    if (e->scancode == sf::Keyboard::Scancode::P)
                    {
                        is_paused_ ^= true;
                    }
                    else if (!is_paused_)
                    {
                        switch (e->scancode)
                        {
                        case sf::Keyboard::Scancode::Down:
                        {
                            GameState state = player_->down_block();
                            process_game_state(state);
                            break;
                        }
                        case sf::Keyboard::Scancode::Left:
                        {
                            player_->left_block();
                            break;
                        }
                        case sf::Keyboard::Scancode::Right:
                        {
                            player_->right_block();
                            break;
                        }
                        case sf::Keyboard::Scancode::Up:
                        {
                            player_->up_block();
                            break;
                        }
                        }
                    }
                }
                else // is game over
                {
                    switch (e->scancode)
                    {
                    case sf::Keyboard::Scancode::Y:
                    {
                        is_game_over_ = false;
                        score_ = 0;
                        nr_of_lines_ = 0;
                        level_ = 1;
                        player_->start_new_game();
                        break;
                    }
                    case sf::Keyboard::Scancode::N:
                    {
                        is_running_ = false;
                        break;
                        // save player data in profile
                        // ...
                    }
                    }
                }
            }
        }
    }
    else
    {
        if (block_clock_.getElapsedTime().asSeconds() >= 0.5f)
        {
            if (!is_game_over_ && !is_paused_)
            {
                GameState state = player_->down_block();
                process_game_state(state);
            }
            block_clock_.restart();
        }
    }
}

void Game::update_score(int nr_of_full_lines)
{
    nr_of_lines_ += nr_of_full_lines;
    switch (nr_of_full_lines)
    {
    case 1:
        score_ += 1;
        break;
    case 2:
        score_ += 3;
        break;
    case 3:
        score_ += 6;
        break;
    case 4:
        score_ += 10;
        break;
    }
}

void Game::update_level()
{
    if (level_ < 9)
    {
        if (score_ % 100 == 0)
        {
            level_++;
            std::cout << "level up ! " << level_ << std::endl;
            level_clock_.restart();
        }
    }
}

void Game::game_over()
{
    player_->update_highscore(score_);
    is_game_over_ = true;
}

void Game::display(const UI &ui) const
{
    if (is_game_over_)
    {
        ui.render_gameover();
    }
    else
    {
        player_->display(ui);
        ui.render_scoreboard(level_, score_, nr_of_lines_);
    }
}

void Game::process_game_state(const GameState &state)
{
    if (state.game_over)
    {
        game_over();
        return;
    }

    if (state.nr_of_full_lines > 0)
    {
        update_score(state.nr_of_full_lines);
        update_level();
        return;
    }
}